// Scorpio Main Code
/* Description and Architecture
*
*
*/

// INCLUDES
/* for some reason I have to include all headers here! TODO: Fix This. Only Include RR_... Headers.
*
*/
#include <RR_CommonData.h>
#include <Encoder.h>
#include <RR_IMU.h>
#include <RR_Telemetry.h>
#include <RR_Altimeter.h>
#include <RR_MOSFET.h>
#include <RR_Encoder.h>
#include <RR_SDCard.h>
#include <RR_Motor.h>
#include <RR_Receiver.h>
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
#include <FreeRTOS_ARM\utility\task.h>
#include <FreeRTOS_ARM\utility\semphr.h>
#include <FreeRTOS_ARM\utility\StackMacros.h>

//ARM doesn't need this. Undef F.
#undef F
#define F(str) str

//-------------OPTIONS------------
DriveMode_t driveMode=MANUAL;
StartMode_t startMode=NAVIGATING;
//---------------------------------

//Global Variables - Ideally each task needs to know the state only, data is exchanged through protected data structs.
MainState_t mainState=startMode;

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
RR_IMU			imu(&IMUData);


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
xSemaphoreHandle GPSSemaphore, IMUSemaphore, TelemetryMutex, LoggerMutex, AltimeterSemaphore;

void setup()
{
	if(DBUG)
	{Serial.begin(115200);}

	GPSData.fix=false;
	AltimeterData.Launched=false; AltimeterData.Peaked=false; AltimeterData.Landed=false;
	
	//Create Tasks
	xTaskCreate(vGPSTask,		(signed portCHAR *)"GPS Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskGPS);
	xTaskCreate(vIMUTask,		(signed portCHAR *)"IMU Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskIMU);
	xTaskCreate(vAltimeterTask, (signed portCHAR *)"Altimeter Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, &_htaskAltiemter);
	xTaskCreate(vDriverTask,	(signed portCHAR *)"Driver Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, &_htaskDriver);
	xTaskCreate(vTelemetryTask, (signed portCHAR *)"Telemetry Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 1, &_htaskTelemetry);
	xTaskCreate(vLoggerTask,	(signed portCHAR *)"Logger Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY, &_htaskLogger);
	xTaskCreate(vStateTask,		(signed portCHAR *)"State Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 1, &_htaskState);

	//Create Binary Semaphore (Binary Semphore used to signal other threads everytime there is new data /
	//and used here to protect access to sensor data
	vSemaphoreCreateBinary(GPSSemaphore);
	vSemaphoreCreateBinary(IMUSemaphore);
	vSemaphoreCreateBinary(AltimeterSemaphore);

	//Create Mutex (Mutex are used to share a protected resouce where multiple threads have write/read access)
	TelemetryMutex = xSemaphoreCreateMutex();
	LoggerMutex = xSemaphoreCreateMutex();

	//Start Tasks
	vTaskStartScheduler();

	//Should not reach here
	Serial.println("Program Exited");
	while(1);
}

void loop()
{/* Nothing to see here. All loops are handled in xTasks */}

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
	// Task is always On!
	// -------------------- //
	while(1){
		if(DBUG) {Serial.println("State Machine Task, State: ");}
		if(DBUG) {Serial.println(mainState);}
		switch(mainState){
			case LAUNCHING:
				SamplingTime=500L;
				if(xSemaphoreTake(AltimeterSemaphore, portMAX_DELAY)){						
					if(AltimeterData.Launched){
						mainState=ASCENDING;
					}
				}
				break;
			case ASCENDING:
				SamplingTime=50L;
				if(xSemaphoreTake(AltimeterSemaphore, portMAX_DELAY)){						
					if(AltimeterData.Peaked){
						mainState=LANDING;
					}
				}
				break;
			case LANDING:
				SamplingTime=200L;
				if(xSemaphoreTake(AltimeterSemaphore, portMAX_DELAY)){						
					if(AltimeterData.Landed){
						//Burn Rope
						nichrome.switchMOSFET(ON); delay(2000); nichrome.switchMOSFET(OFF);
						mainState=LANDED;
					}
				}
				break;
			case LANDED:
				SamplingTime=2000L;
				if(xSemaphoreTake(GPSSemaphore, portMAX_DELAY)){
					if(GPSData.fix){
						mainState=NAVIGATING;
					}
				}
				break;
			case NAVIGATING:
				SamplingTime=500L;
				if(xSemaphoreTake(GPSSemaphore, portMAX_DELAY)){
					if(GPSData.DistanceToTarget<10){
						//mainState=FINISHED;
					}
				}
				break;
			case FINISHED:
				SamplingTime=5000L;
				break;
		}
		xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
		TelemetryOutgoingData.LaunchState=mainState;
		xSemaphoreGive(TelemetryMutex);
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
	uint16_t SamplingTime= 2000L; //2 seconds - Rover is not that fast! Same for both taskOn and Idle.
	bool taskOn=false;
	// -------------------- //
	uint16_t latTest=0;
	while(1)
	{
		switch(mainState)
		{
			case LAUNCHING: case ASCENDING: case LANDED: case FINISHED:
				if(taskOn){
					gps.Disable();
					taskOn=false;
				}
				break;
			case LANDING: case NAVIGATING:
				if(!taskOn){
					gps.Enable();
					//enable;
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
				xSemaphoreGive(GPSSemaphore);
			}
			else{
				if(DBUG){Serial.println("NoFix");}
				xSemaphoreGive(GPSSemaphore);
			}		
			//Update Relevant Telemetry Data

			if(xSemaphoreTake(TelemetryMutex, portMAX_DELAY)){
			memcpy(&TelemetryOutgoingData.Date, &GPSData.Date, sizeof(GPSData.Date));
			memcpy(&TelemetryOutgoingData.Time, &GPSData.Time, sizeof(GPSData.Time));
			TelemetryOutgoingData.Latitude=GPSData.Latitude;
			TelemetryOutgoingData.Longitude=GPSData.Longitude;
			TelemetryOutgoingData.DistanceToTarget=GPSData.DistanceToTarget;
			TelemetryOutgoingData.Bearing=GPSData.Bearing;
			xSemaphoreGive(TelemetryMutex);
			}

			//Update Relevant Logger Data
			if(xSemaphoreTake(LoggerMutex, portMAX_DELAY)){
			memcpy(&LoggerData.GPSTime, &GPSData.Time, sizeof(GPSData.Date));
			LoggerData.GPS.Latitude=GPSData.Latitude;
			LoggerData.GPS.Lat=GPSData.Lat;
			LoggerData.GPS.Longitude=GPSData.Longitude;
			LoggerData.GPS.Lon=GPSData.Lon;
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
	// -------------------- //

	//Change this to a switch case inside one whole while loop
	while(1){
		//
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
					SamplingTime= 500L;
					imu.initIMU();
				}
				break;
		}

		if(taskOn){
			if(0) {Serial.println("IMU Task");}
			imu.updateIMU();
			xSemaphoreGive(IMUSemaphore);
			//Update Relevant Telemetry Data
			xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
			//No need to take IMUSephore since it won't be updated anywhere else but this thread. Is this safe?
			TelemetryOutgoingData.Heading=IMUData.fused.heading;
			xSemaphoreGive(TelemetryMutex);

			//Update Relevant Logger Data
			xSemaphoreTake(LoggerMutex, portMAX_DELAY);
			//LoggerData.... = ...
			xSemaphoreGive(LoggerMutex);
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
	while(1){
		switch (mainState){
			case LAUNCHING:
				if(!taskOn){
					taskOn=true;
					altimeter.initAltimeter();
				}
				altimeterTask=checkLAUNCH;
				SamplingTime= 500L;
				break;
			case ASCENDING:
				if(!taskOn){
					taskOn=true;
					altimeter.initAltimeter();
				}
				altimeterTask=checkPEAK;
				SamplingTime= 20L;
				break;
			case LANDING:
				if(!taskOn){
					taskOn=true;
					altimeter.initAltimeter();
				}
				SamplingTime= 100L;
				altimeterTask=checkLANDING;
				break;
			case LANDED: case NAVIGATING: case FINISHED:
				if(taskOn){
					taskOn=false;
					//altimeter.sleep()
					SamplingTime= 500L;
				}
			break;
		}

		if(taskOn){
			if(DBUG) {Serial.println("Altimeter Task");}
			altimeter.updateAltimeter(altimeterTask, SamplingTime); //Depending on the task the update function will perform it and update the
													//data struct flags accordingly. 
			xSemaphoreGive(AltimeterSemaphore);
	
			//Update Relevant Telemetry Data
			xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
			TelemetryOutgoingData.Altitude=AltimeterData.altitude;
			xSemaphoreGive(TelemetryMutex);

			//Update Relevant Logger Data
			xSemaphoreTake(LoggerMutex, portMAX_DELAY);
			LoggerData.Altitude=AltimeterData.altitude;
			xSemaphoreGive(LoggerMutex);
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
	// -------------------- //
	while(1){
		switch(mainState){
			case LAUNCHING: case ASCENDING: case LANDING: case LANDED:
				if(taskOn){
					SamplingTime=500L;
					taskOn=false;
				}
				break;
			case NAVIGATING:
				if(!taskOn){
						driver.Enable();
						taskOn=true;
				}
				if(DBUG) {Serial.println("Driver Task");}
				if(driveMode==MANUAL){
					driver.driveManual();
				}
				else if(driveMode==AUTONOMOUS_SIMPLE){
					//TODO: Figure out an efficient way to take and wait for multiple semaphores. 
//					driver.driveAutonomous(NavigationData, GPSData, IMUData, LoggerData);
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
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	// -------------------- //
	while(1)
	{
		//Serial.println("Telemetry Task");
		xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
		radio.Update();
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
	uint16_t SamplingTime=500L;  //Sampling Time (ms) - Initially set at idle
	// -------------------- //
	sdcard.Initialize();
	while(1)
	{
		//Serial.println("Logger Task");
		xSemaphoreTake(LoggerMutex, portMAX_DELAY);
		sdcard.updateLog();
		xSemaphoreGive(LoggerMutex);
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}


