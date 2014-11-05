// Scorpio Main Code
/* Description and Architecture
*
*
*/

// INCLUDES
/* for some reason I have to include all headers here! TODO: Fix This. Only Include RR_... Headers.
*
*/


#include <EEPROM.h>
#include <Filter.h>
#include <vector.h>
#include <quaternion.h>
#include <matrix.h>
#include <imumaths.h>
#include <ahrs.h>
#include <RR_CommonData.h>
#include <RR_IMU.h>
#include <RR_Telemetry.h>
#include <RR_Altimeter.h>
#include <RR_MOSFET.h>
#include <RR_SDCard.h>
#include <RR_Motor.h>
#if USE_RECEIVER
#include <RR_Receiver.h>
#else
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <RR_Encoder.h>
#endif

#include <RR_Driver.h>
#include <RR_GPS.h>
#include <RR_CommonDefines.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_GPS.h>
#include <FreeRTOS_ARM.h>
#include <basic_io_arm.h>
#include <Wire.h>

//Must include libraries not directly in "libraries" folder explicitly to compile with arduino ide
#include "..\FreeRTOS_ARM\utility\task.h"
#include "..\FreeRTOS_ARM\utility\semphr.h"
#include "..\FreeRTOS_ARM\utility\StackMacros.h"
//#include "..\FreeRTOS_ARM\utility\queue.h"




//ARM doesn't need this. Undef F.
#undef F
#define F(str) str

//-------------OPTIONS------------
//#define USE_EEPROM
uint8_t * stateEEPROMaddr = (uint8_t *)0x01;
uint8_t driveMode=
	MANUAL_PC;
//Define Start Mode, works only when not retrieving state from EEPROM
StartMode_t startMode= 
	NAVIGATING;
//---------------------------------

//Global Variables - Ideally each task needs to know the state only, data is exchanged through protected data structs.
MainState_t mainState;
//Shared Data Structs

RR_IMUData_t					IMUData;
RR_AltimeterData_t				AltimeterData;
RR_LoggerData_t					LoggerData;
RR_GPSData_t					GPSData;
RR_NavigationData_t				NavigationData;
RR_TelemetryIncomingMessage_t	TelemetryIncomingData;
RR_TelemetryOutgoingMessage_t	TelemetryOutgoingData;


// List of Objects
/* Description:
*
*/
RR_Altimeter	altimeter(&AltimeterData);
RR_Driver		driver(&NavigationData);
RR_GPS			gps(&GPSData);
RR_MOSFET		nichrome;
RR_SDCard		sdcard(&LoggerData); 
RR_Telemetry	radio(&TelemetryOutgoingData,&TelemetryIncomingData);
RR_IMU			imuS(&IMUData);


//List of Task Functions - Creations / Definitions Below
static void vGPSTask(void *pvParameters);
static void vIMUTask(void *pvParameters);
static void vAltimeterTask(void *pvParameters);
static void vDriverTask(void *pvParameters);
static void vTelemetryTask(void *pvParameters);
static void vLoggerTask(void *pvParameters);
static void vStateTask (void *pvParameters);
//Task Handles
xTaskHandle _htaskGPS, _htaskIMU, _htaskAltiemter,  _htaskDriver, _htaskTelemetry, _htaskLogger, _htaskState;
//Semaphore/Mutex Handles
xSemaphoreHandle GPSSemaphore, IMUSemaphore, TelemetryMutex, LoggerMutex, IMUAccessMutex, AltimeterSemaphore;

void setup()
{

	#ifdef USE_EEPROM
	mainState=(MainState_t)eeprom_read_byte(stateEEPROMaddr);
	#else
	 mainState=startMode;
	 eeprom_write_byte(stateEEPROMaddr,LAUNCHING);
	#endif
	
	if(DBUG)
	{Serial.begin(115200);}
	
	GPSData.fix=false;
	AltimeterData.Launched=false; AltimeterData.Peaked=false; AltimeterData.Landed=false;


	//Create Tasks
	xTaskCreate(vGPSTask,		(signed portCHAR *)"GPS Task",			configMINIMAL_STACK_SIZE + 300, NULL, tskIDLE_PRIORITY + 2, &_htaskGPS);
	xTaskCreate(vIMUTask,		(signed portCHAR *)"IMU Task",			configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &_htaskIMU);
	xTaskCreate(vAltimeterTask, (signed portCHAR *)"Altimeter Task",	configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &_htaskAltiemter);
	xTaskCreate(vDriverTask,	(signed portCHAR *)"Driver Task",		configMINIMAL_STACK_SIZE + 200, NULL, tskIDLE_PRIORITY + 2, &_htaskDriver);
	xTaskCreate(vTelemetryTask, (signed portCHAR *)"Telemetry Task",	configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &_htaskTelemetry);
	xTaskCreate(vLoggerTask,	(signed portCHAR *)"Logger Task",		configMINIMAL_STACK_SIZE + 1000, NULL, tskIDLE_PRIORITY + 2, &_htaskLogger);
	xTaskCreate(vStateTask,		(signed portCHAR *)"State Task",		configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &_htaskState);
	delay(2000);

	//Create Binary Semaphore (Binary Semphore used to signal other threads everytime there is new data /
	//and used here to protect access to sensor data
	vSemaphoreCreateBinary(GPSSemaphore);
	vSemaphoreCreateBinary(IMUSemaphore);
	vSemaphoreCreateBinary(AltimeterSemaphore);

	//Create Mutex (Mutex are used to share a protected resouce where multiple threads have write/read access)
	TelemetryMutex	= xSemaphoreCreateMutex();
	LoggerMutex		= xSemaphoreCreateMutex();
	IMUAccessMutex	= xSemaphoreCreateMutex();

	//Start Tasks
	vTaskStartScheduler();

	//Should not reach here
	Serial.println("Program Exited");
	while(1);
}

void loop()
{
/* Nothing to see here. All loops are handled in xTasks */}

// STATE MACHINE TASK
/*
* Description: This task will observe changes in states and update the state flag accordingly.
* This task controls how other tasks behave.
*
*/
static void vStateTask (void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	uint8_t fixConfirmInd=0;
	GPSData.DistanceToTarget=10000;
	// Task is always On!
	// -------------------- //
	while(1){
		if(DBUG) {Serial.print("S:");}
		if(DBUG) {Serial.println(mainState);}
		switch(mainState){
			case LAUNCHING:
				SamplingTime=500L;
				if(xSemaphoreTake(AltimeterSemaphore, portMAX_DELAY)){						
					if(AltimeterData.Launched){
						eeprom_write_byte(stateEEPROMaddr,ASCENDING);
						mainState=ASCENDING;
					}
				}
				break;
			case ASCENDING:
				SamplingTime=100L;
				if(xSemaphoreTake(AltimeterSemaphore, portMAX_DELAY)){						
					if(AltimeterData.Peaked){
						eeprom_write_byte(stateEEPROMaddr,LANDING);
						mainState=LANDING;
						if(xSemaphoreTake(TelemetryMutex, 100/ portTICK_RATE_MS)){
						TelemetryOutgoingData.maxAltitude=AltimeterData.maxAltitude;
						xSemaphoreGive(TelemetryMutex);
						}

						if(xSemaphoreTake(LoggerMutex, 100/portTICK_RATE_MS)){
						sdcard.WriteMessage(AltimeterData.maxAltitude,"$MaxAlt: ");
						xSemaphoreGive(LoggerMutex);
						}
					}
				}
				break;
			case LANDING:
				SamplingTime=100L;
				if(xSemaphoreTake(AltimeterSemaphore, 100/portTICK_RATE_MS)){						
					if(AltimeterData.Landed){
						//Burn Rope
						if(DBUG2){Serial.println("Burn Rope and wait a few sec");}
						nichrome.switchMOSFET(ON); delay(3000); nichrome.switchMOSFET(OFF);
						eeprom_write_byte(stateEEPROMaddr,LANDED);
						delay(10000);
	
						mainState=LANDED;
					}
				}
				break;
			case LANDED:
				SamplingTime=2000L;
				if(xSemaphoreTake(GPSSemaphore, 200/portTICK_RATE_MS)){
					if(GPSData.fix){
						fixConfirmInd++;

					}
					else{
						fixConfirmInd=0;
					}
					if(fixConfirmInd>3){
						eeprom_write_byte(stateEEPROMaddr,NAVIGATING);
						mainState=NAVIGATING;
					}
				}
				break;
			case NAVIGATING:
				SamplingTime=500L;
				if(xSemaphoreTake(GPSSemaphore, 200/portTICK_RATE_MS)){
					if(DBUG){Serial.println("NAV:GPS");}
					if(GPSData.DistanceToTarget<10){
						eeprom_write_byte(stateEEPROMaddr,FINISHED);
						mainState=FINISHED;
					}
				}
				break;
			case FINISHED:
				SamplingTime=5000L;
				break;
		}
		
		if(xSemaphoreTake(TelemetryMutex, portMAX_DELAY)){
			TelemetryOutgoingData.LaunchState=mainState;
		xSemaphoreGive(TelemetryMutex);
		}

		if(xSemaphoreTake(LoggerMutex, 200/portTICK_RATE_MS)){
			LoggerData.State.mainstate=mainState;
		xSemaphoreGive(LoggerMutex);
		}
		
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

// GPS TASK
/*
* Description: This task will read and parse a new GPS message at the set sampling time\
* and update GPSData struct accordingly. It will also update the Telemetry and Logger structs\
* with relevant data.
*/
static void vGPSTask(void *pvParameters){
	// --- Task Options --- //
	uint16_t SamplingTime= 1000L; //2 seconds - Rover is not that fast! Same for both taskOn and Idle.
	bool taskOn=false;
	uint32_t lastmillis = millis();
	int16_t lastDistanceTravelled = 0;
	// -------------------- //
	uint16_t latTest=0;
	while(1)
	{
		switch(mainState)
		{
			case LAUNCHING: case ASCENDING:  
				if(taskOn){
					gps.Disable();
					taskOn=false;
				}
				break;
			 case LANDING: case LANDED: case NAVIGATING: case FINISHED:
				if(!taskOn){
					gps.Enable();
					taskOn=true;
				}
				break;
		}
		if(taskOn)
		{
			if(DBUG) {Serial.println("GPS Task");}
			while(!gps.newGPSData){
				gps.Update();
			}
			gps.newGPSData=false;
			if(gps.fix){
				gps.getData();
				if(DBUG2){Serial.println("Fix");
				Serial.print("Distance Travelled: "); Serial.println(GPSData.DistanceTravelled);
				/*Serial.print("Dist 2 Tgt: ");
				Serial.println(GPSData.DistanceToTarget);
				Serial.print("Bearing: ");
				Serial.println(GPSData.Bearing);*/
				}
				xSemaphoreGive(GPSSemaphore);
			}
			else{
				if(DBUG2){Serial.println("NoFix");}
				xSemaphoreGive(GPSSemaphore);
			}

			//Check if not moving
			if(millis()-lastmillis>MOVING_SAMPLING_T){
				if(GPSData.DistanceTravelled-lastDistanceTravelled<MOVING_DISTANCE_THRESHOLD){
					GPSData.isMoving = 0;
					if(DBUG2){Serial.println("Not Moving");}
				}
				else{
					GPSData.isMoving = 1;
				}
				lastDistanceTravelled = GPSData.DistanceTravelled;
				lastmillis=millis();
			}
			//Update Relevant Telemetry Data

			if(xSemaphoreTake(TelemetryMutex, portMAX_DELAY)){
			memcpy(&TelemetryOutgoingData.Date, &GPSData.Date, sizeof(GPSData.Date));
			memcpy(&TelemetryOutgoingData.Time, &GPSData.Time, sizeof(GPSData.Time));
			if(DBUG){Serial.print("Fix: "); Serial.println(GPSData.fix);}
			TelemetryOutgoingData.fix=GPSData.fix;
			TelemetryOutgoingData.Latitude=GPSData.Latitude;
			TelemetryOutgoingData.Longitude=GPSData.Longitude;
			TelemetryOutgoingData.targetLatidude=GPSData.targetLatidude;
			TelemetryOutgoingData.targetLongitude=GPSData.targetLongitude;
			TelemetryOutgoingData.DistanceTravelled=GPSData.DistanceTravelled;
			TelemetryOutgoingData.DistanceToTarget=GPSData.DistanceToTarget;
			TelemetryOutgoingData.Bearing=GPSData.Bearing;
			xSemaphoreGive(TelemetryMutex);
			}

			//Update Relevant Logger Data
			if(xSemaphoreTake(LoggerMutex, 200/portTICK_RATE_MS)){
			memcpy(&LoggerData.GPSTime, &GPSData.Time, sizeof(GPSData.Date));
			LoggerData.GPS.fix=GPSData.fix;
			LoggerData.GPS.Latitude=GPSData.Latitude;
			LoggerData.GPS.Lat=GPSData.Lat;
			LoggerData.GPS.Longitude=GPSData.Longitude;
			LoggerData.GPS.Lon=GPSData.Lon;
			LoggerData.GPS.DistanceTravelled=GPSData.DistanceTravelled;
			LoggerData.GPS.DistanceToTarget=GPSData.DistanceToTarget;
			LoggerData.GPS.Bearing=GPSData.Bearing;
			xSemaphoreGive(LoggerMutex);
			}
		}
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

// IMU TASK
/*
* Description: 
*
*/
static void vIMUTask(void *pvParameters){
	// --- Task Options --- //
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	bool taskOn=false;
	Filter OrientationF;
	int fvcounter=0;
	// -------------------- //

	//Change this to a switch case inside one whole while loop
	
	while(1){
		
		switch(mainState){
			case LAUNCHING: case ASCENDING: case LANDING: case LANDED: case FINISHED:
				if(taskOn){
					SamplingTime=500L; //Idle Thread Sampling Time
					//imu.disable() or sleep() -- TODO: Implement such function
					taskOn=false;
				}
				break;
			case NAVIGATING:
				if(!taskOn){
					taskOn=true;
					SamplingTime= 10L;
					if(xSemaphoreTake(IMUAccessMutex, portMAX_DELAY)){		
						imuS.initIMU();
						xSemaphoreGive(IMUAccessMutex);
					}

				}
				break;
		}

		if(taskOn){
			if(0) {Serial.println("IMU Task");}
			if(xSemaphoreTake(IMUAccessMutex, portMAX_DELAY)){		
				imuS.updateIMU();
				xSemaphoreGive(IMUAccessMutex);
			}

			//Update Relevant Telemetry Data
			
			if(xSemaphoreTake(TelemetryMutex, portMAX_DELAY)){
			//No need to take IMUSephore since it won't be updated anywhere else but this thread. Is this safe?
			//TelemetryOutgoingData.Heading=IMUData.fused.heading;
			TelemetryOutgoingData.Heading=IMUData.headingFiltered;
			xSemaphoreGive(TelemetryMutex);
			}
			IMUData.headingFiltered=-OrientationF.step(IMUData.fused.heading);
			fvcounter++;
			if(SamplingTime*fvcounter>500L){
				fvcounter=0;

				if(0) {Serial.print("Orientation: ");
				Serial.println(IMUData.headingFiltered);
				}
				xSemaphoreGive(IMUSemaphore);

				//Update Relevant Logger Data
				
				if(xSemaphoreTake(LoggerMutex, 200/portTICK_RATE_MS)){
					LoggerData.Navigation.heading=IMUData.headingFiltered;
					//LoggerData.... = ...
				xSemaphoreGive(LoggerMutex);
				}
				
			}
		}
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);	
	}
}

// ALTIMETER TASK
/*
* Description
*
*/
static void vAltimeterTask(void *pvParameters){	
	// --- Task Options --- //
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	bool taskOn=false;
	AltimeterTask_t altimeterTask=STANDBY;
	// -------------------- //
	uint16_t telemetryUpdateCounter=0;
	//delay(1000);


	if(xSemaphoreTake(LoggerMutex, portMAX_DELAY)){
	sdcard.WriteMessage(AltimeterData.baseAltitude,"$BaseAlt: ");
	xSemaphoreGive(LoggerMutex);
	}

	while(1){
		switch (mainState){
			case LAUNCHING:
				if(!taskOn){
					taskOn=true;
					if(xSemaphoreTake(IMUAccessMutex, portMAX_DELAY)){		
						altimeter.initAltimeter();
						if(xSemaphoreTake(TelemetryMutex, portMAX_DELAY)){
						TelemetryOutgoingData.baseAltitude=AltimeterData.baseAltitude;
						xSemaphoreGive(TelemetryMutex);
						}
						xSemaphoreGive(IMUAccessMutex);
					}
				}
				altimeterTask=checkLAUNCH;
				SamplingTime= 500L;
				break;
			case ASCENDING:
				if(!taskOn){
					taskOn=true;
				}
				altimeterTask=checkPEAK;
				SamplingTime= 20L;
				break;
			case LANDING:
				if(!taskOn){
					taskOn=true;
				}
				SamplingTime= 50L;
				altimeterTask=checkLANDING;
				break;
			case LANDED: case NAVIGATING: case FINISHED:
				if(!taskOn){
					taskOn=true;

				}
				altimeterTask=STANDBY;
				SamplingTime= 1000L;
				break;
		}

		if(taskOn){
			if(0) {Serial.println("Altimeter Task");}
			if(xSemaphoreTake(IMUAccessMutex, portMAX_DELAY)){		
				altimeter.updateAltimeter(altimeterTask, SamplingTime); //Depending on the task the update function will perform it and update the
													//data struct flags accordingly. 
				if(0){Serial.print("M:"); Serial.println(millis());}
				xSemaphoreGive(IMUAccessMutex);
			}

			xSemaphoreGive(AltimeterSemaphore);
	
			//Update Relevant Telemetry Data
			telemetryUpdateCounter++;
			if(SamplingTime*telemetryUpdateCounter>1000L){
				telemetryUpdateCounter=0;
				xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
				TelemetryOutgoingData.Altitude=AltimeterData.altitude;
				xSemaphoreGive(TelemetryMutex);
			}

			//Update Relevant Logger Data
			
			if(xSemaphoreTake(LoggerMutex, 50/portTICK_RATE_MS)){
			LoggerData.Altitude=AltimeterData.altitude;
			xSemaphoreGive(LoggerMutex);
			}
			
		}
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);	
	}
}

// DRIVER TASK
/*
* Description: This task will 
*
*/
static void vDriverTask(void *pvParameters){

	// --- Task Options --- //
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	bool taskOn=false;
	RR_GPSData_t gpsDataNav;
	RR_IMUData_t imuDataNav;
	RR_LoggerData_t loggerDataNav;
	RR_NavigationData_t navDataNav;
	uint16_t		loggerSamplingCounter=0;
	double			lastMillis=0;
	gpsDataNav.Bearing=0;
	// -------------------- //
	while(1){
		switch(mainState){
			case LAUNCHING: case ASCENDING: case LANDING:
				if(taskOn){
					SamplingTime=500L;
					taskOn=false;
				}
				break;
			case LANDED:
				if(!taskOn){
						driver.Enable();
						taskOn=true;
						driver.setFree();
				}
				SamplingTime=500L;
				break;
			case NAVIGATING:
				if(!taskOn){
						driver.Enable();
						taskOn=true;		
				}
				SamplingTime=20L;
				//if(DBUG) {Serial.println("Driver Task");}
				if(driveMode==MANUAL_3CH){
					driver.driveManual(uint16_t(millis()-lastMillis));

					lastMillis=millis();
				}

				else if(driveMode==MANUAL_PC){
					
					if(xSemaphoreTake(GPSSemaphore, 25 / portTICK_RATE_MS)){
						if(GPSData.fix){
						//memcpy(&gpsDataNav,&GPSData,sizeof(GPSData));	
							gpsDataNav.Bearing=GPSData.Bearing;
							gpsDataNav.isMoving=GPSData.isMoving;
							if(DBUG){Serial.println(gpsDataNav.Bearing);}
						}
					}
					
					if(xSemaphoreTake(IMUSemaphore, 25 / portTICK_RATE_MS)){
						memcpy(&imuDataNav,&IMUData,sizeof(IMUData));
					}
					
					xSemaphoreTake(TelemetryMutex, 25 / portTICK_RATE_MS);
					joystick_t joystick;
					memcpy(&joystick,&TelemetryIncomingData.Joystick,sizeof(TelemetryIncomingData.Joystick));
					int16_t actualSpeedLeft, actualSpeedRight;
					uint16_t leftCurrent, rightCurrent;
					driver.driveManual(joystick, gpsDataNav, imuDataNav, uint16_t(millis()-lastMillis));
					driver.getMotorData(&actualSpeedLeft,&actualSpeedRight,&leftCurrent,&rightCurrent);
					lastMillis=millis();
					TelemetryOutgoingData.Motors.leftCurrent=leftCurrent;
					TelemetryOutgoingData.Motors.rightCurrent=rightCurrent;
					TelemetryOutgoingData.Motors.leftSpeed=actualSpeedLeft;
					TelemetryOutgoingData.Motors.rightSpeed=actualSpeedRight;
					xSemaphoreGive(TelemetryMutex);
				}
				
				else if(driveMode==AUTONOMOUS_SIMPLE){
					
					if(xSemaphoreTake(GPSSemaphore, 50 / portTICK_RATE_MS)){
							if(GPSData.fix){
							//memcpy(&gpsDataNav,&GPSData,sizeof(GPSData));
								
								gpsDataNav.Bearing=GPSData.Bearing;
								gpsDataNav.isMoving=GPSData.isMoving;
								if(DBUG){Serial.println(gpsDataNav.Bearing);}
							}

						}
					
					if(xSemaphoreTake(IMUSemaphore, 50 / portTICK_RATE_MS)){
						
						//Serial.println("Navigation: IMU Semaphore Taken");
						memcpy(&imuDataNav,&IMUData,sizeof(IMUData));
					}
					
					driver.driveAutonomous(gpsDataNav, imuDataNav, loggerDataNav, uint16_t(millis()-lastMillis));
					lastMillis=millis();
					
					loggerSamplingCounter++;
					if(SamplingTime*loggerSamplingCounter>5000L){
						loggerSamplingCounter=0;
						if(xSemaphoreTake(LoggerMutex, 200 / portTICK_RATE_MS)){
							if(0){Serial.println("Navigation: Copy to logger data");}
							LoggerData.Navigation.speedleft=loggerDataNav.Navigation.speedleft;
							LoggerData.Navigation.speedright=loggerDataNav.Navigation.speedright;
							xSemaphoreGive(LoggerMutex);
						}
					}
				}
				break;
			case FINISHED:
				driver.Stop(); //Replace with Disable Board
				break;
		}
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

// TELEMETRY TASK
/*
* Description: This task will send/receive updates through the radio. 
*
*/
static void vTelemetryTask(void *pvParameters){
	// --- Task Options --- //
	uint16_t SamplingTime=100L;  //Sampling Time (ms) - Initially set at idle
	bool mosfetSwitched = false;
	// -------------------- //
	//TODO: only enable after launched
	while(1)
	{
		switch(mainState)
		{
			case LAUNCHING: 
				SamplingTime=1000L;
				break;
			case ASCENDING:
				SamplingTime=100L;
				break;
			case LANDING:
				SamplingTime=500L;
				break;
			case LANDED:
				SamplingTime=1000L;
				break;
			case NAVIGATING:
				if(driveMode==MANUAL_PC){
					SamplingTime=150L;			
				}
				else{
					if(DBUG){
						SamplingTime=1000L;
					}
					else{
						SamplingTime=5000L;
					}
				}
				break;
			case FINISHED:
				SamplingTime=5000L;
				break;
		}
		//Serial.println("Telemetry Task");
		xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
		TelemetryOutgoingData.ellapsedm=millis()/1000;
		radio.Update();
		if(TelemetryIncomingData.bottum1){
			Serial.println("B Pressed");
			//if(!mosfetSwitched){
				mosfetSwitched=true;
				Serial.println("Mosfet Switched");
				nichrome.switchMOSFET(ON);
				delay(2000);
				nichrome.switchMOSFET(OFF);
				TelemetryIncomingData.bottum1=false;
			//}

		}

		xSemaphoreGive(TelemetryMutex);
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

// LOGGER TASK
/*
* Description: This task will log data onto the SD Card.
*
*/
static void vLoggerTask(void *pvParameters){
	// --- Task Options --- //
	uint16_t SamplingTime=100L;  //Sampling Time (ms) - Initially set at idle
	WhatToLog_t whatToLog;
	whatToLog.altitude=false; whatToLog.gps=false; whatToLog.imu=false; whatToLog.navigation=false;
	// -------------------- //
	if(xSemaphoreTake(LoggerMutex, portMAX_DELAY)){
		sdcard.Initialize();
		xSemaphoreGive(LoggerMutex);
	}
	while(1)
	{
		switch(mainState)
		{
			case LAUNCHING: 
				whatToLog.altitude=true;	whatToLog.gps=false;	whatToLog.imu=false;	whatToLog.navigation=false;
				SamplingTime=500L;
				break;
			case ASCENDING:
				whatToLog.altitude=true;	whatToLog.gps=false;	whatToLog.imu=false;	whatToLog.navigation=false;
				SamplingTime=100L;
				break;
			case LANDING:
				whatToLog.altitude=true;	whatToLog.gps=false;	whatToLog.imu=false;	whatToLog.navigation=false;
				SamplingTime=500L;
				break;
			case LANDED:
				whatToLog.altitude=true;	whatToLog.gps=false;	whatToLog.imu=false;	whatToLog.navigation=false;
				SamplingTime=1000L;
				break;
			case NAVIGATING:
				whatToLog.altitude=false;	whatToLog.gps=true;		whatToLog.imu=true;		whatToLog.navigation=true;
				SamplingTime=10000L;
				break;
			case FINISHED:
				whatToLog.altitude=false;	whatToLog.gps=true;		whatToLog.imu=false;	whatToLog.navigation=false;
				SamplingTime=30000L;
				break;
		}
		//Serial.println("Logger Task");
		if(xSemaphoreTake(LoggerMutex, portMAX_DELAY)){
			sdcard.updateLog(&whatToLog);
			xSemaphoreGive(LoggerMutex);
		}
		
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}


