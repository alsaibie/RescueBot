/*
* Scorpio Main Code
*
*
*/

//Includes - for some reason I have to include all headers here! TODO: Fix This. Only Include RR_... Headers.
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

//List of Objects
RR_Altimeter	altimeter(&AltimeterData);
RR_Driver		driver;
RR_GPS			gps(&GPSData);
RR_MOSFET		nichrome;
//RR_Encoder		encoder; //only needed inside Driver class
//RR_SDCard		sdcard; //Class needs fixing
RR_Telemetry	radio(&TelemetryOutgoingData,&TelemetryIncomingData);  //Class needs making
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

/*
* STATE MACHINE TASK
* Description: This task will observe changes in states and update the state flag accordingly.
* This task controls how other tasks behave.
*
*/

static void vStateTask (void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime= 50L; //50 msecond
	// -------------------- //
	while(1)
	{
		if(DBUG) {Serial.println("State Machine Task");}
	
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

/*
* GPS TASK
*
*
*/
static void vGPSTask(void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime= 2000L; //2 seconds - Rover is not that fast! 
	// -------------------- //

	while(1)
	{
		//Serial.println("GPS Task");
		//Update Relevant Telemetry Data
		xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
		
		xSemaphoreGive(TelemetryMutex);

		//Delay Task for 2 Seconds
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}

}

/*
* IMU TASK
*
*
*/

static void vIMUTask(void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime= 50L;  //Start Sampling Rate
	// -------------------- //
	//Change this to a switch case inside one whole while loop
	while(1)
	{
		switch(mainState)
		{
			case LAUNCHING:
			case ASCENDING:
			case LANDING:
			case LANDED:
				break;
			case NAVIGATING:
				imu.initIMU();
				while(mainState==NAVIGATING)
				{
					if(DBUG) {Serial.println("IMU Task");}
					imu.updateIMU();
					xSemaphoreGive(IMUSemaphore);
	
					//Update Relevant Telemetry Data
					xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
					//No need to take IMUSephore since it won't be updated anywhere else but this thread. Is this safe?
					TelemetryOutgoingData.heading=IMUData.fused.heading;
					xSemaphoreGive(TelemetryMutex);
				}
				break;
			case FINISHED:
				break;
		}

		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);	
	}

	while(mainState<NAVIGATING){}
	
	while(mainState==NAVIGATING)
	{

	}
}

/*
* ALTIMETER TASK
*
*
*/

static void vAltimeterTask(void *pvParameters)
{	
	// --- Task Options --- //
	uint16_t SamplingTime= 100L; 
	// -------------------- //
	while(1)
	{
		//altimeter.initAltimeter();
		//Serial.println("Altimeter Task");
		//altimeter.updateIMU();
		xSemaphoreGive(AltimeterSemaphore);
		//Delay Task for 100ms
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);	
	}
}

/*
* DRIVER TASK
*
*
*/
static void vDriverTask(void *pvParameters)
{

	// --- Task Options --- //
	uint16_t SamplingTime= 100L; //100 ms
	// -------------------- //
	//Wait Until We Reach Navigation State
	while(mainState<NAVIGATING){};
	driver.Enable();
	while(mainState==NAVIGATING)
	{
		if(DBUG)
		{Serial.println("Driver Task");}
		if(driveMode==MANUAL)
		{
			driver.driveManual();
		}
		else if(driveMode==AUTONOMOUS_SIMPLE)
		{
			//TODO: Figure out an efficient way to take and wait for multiple semaphores. 
			driver.driveAutonomous(NavigationData, GPSData, IMUData, LoggerData);
		}
		
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
	
}

/*
* TELEMETRY TASK
* Description: This task will send/receive updates through the radio. 
*
*/

static void vTelemetryTask(void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime= 100L; //100 ms
	// -------------------- //
	while(1)
	{
		//Serial.println("Telemetry Task");
		xSemaphoreTake(TelemetryMutex, portMAX_DELAY);
		radio.Update();
		xSemaphoreGive(TelemetryMutex);
		
		//Delay Task
		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}

/*
* LOGGER TASK
* Description: This task will log data onto the SD Card.
*
*/

static void vLoggerTask(void *pvParameters)
{
	// --- Task Options --- //
	uint16_t SamplingTime= 1000L; //1 second
	// -------------------- //
	while(1)
	{
		//Serial.println("Logger Task");
		xSemaphoreTake(LoggerMutex, portMAX_DELAY);
		//log.update
		xSemaphoreGive(LoggerMutex);

		vTaskDelay((SamplingTime * configTICK_RATE_HZ) / 1000L);
	}
}


