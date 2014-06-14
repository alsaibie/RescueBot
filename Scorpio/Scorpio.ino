
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

#undef F
#define F(str) str

//Task Handles
xTaskHandle _htaskGPS, _htaskIMU, _htaskAltiemter,  _htaskDriver, _htaskTelemetry, _htaskLogger, _htaskState;
//Queue Handles
xQueueHandle _queuehandle1;
//Queues to create: Need to think about the message passing structure first.
//Semaphore Handles
xSemaphoreHandle _semaphorehandle1;

//List of Tasks - Definitions
static void vGPSTask(void *pvParameters);
static void vIMUTask(void *pvParameters);
static void vAltimeterTask(void *pvParameters);
static void vDriverTask(void *pvParameters);
static void vTelemetryTask(void *pvParameters);
static void vLoggerTask(void *pvParameters);
static void vStateTask (void *pvParameters);

//List of Objects
RR_Altimeter	altimeter;
RR_Driver		driver;
RR_GPS			gps;
RR_MOSFET		nichrome;
RR_Encoder		encoder;
//RR_SDCard		sdcard; //Class needs fixing
//RR_Telemetry	radio;  //Class needs making
RR_IMU			imu;

void setup()
{

Serial.begin(115200);

xTaskCreate(vGPSTask,		(signed portCHAR *)"GPS Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskGPS);
xTaskCreate(vIMUTask,		(signed portCHAR *)"IMU Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskIMU);
xTaskCreate(vAltimeterTask, (signed portCHAR *)"Altimeter Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, &_htaskAltiemter);
xTaskCreate(vDriverTask,	(signed portCHAR *)"Driver Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 3, &_htaskDriver);
xTaskCreate(vTelemetryTask, (signed portCHAR *)"Telemetry Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 1, &_htaskTelemetry);
xTaskCreate(vLoggerTask,	(signed portCHAR *)"Logger Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY, &_htaskLogger);
xTaskCreate(vStateTask,	(signed portCHAR *)"State Machine Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 1, &_htaskState);
// start FreeRTOS
vTaskStartScheduler();

//Should not reach here
Serial.println("Program Exited");
while(1);
}

void loop()
{

  /* Nothing to see here. All tasks are handled in xTasks */

}

static void vGPSTask(void *pvParameters)
{
	while(1)
	{
		Serial.println("GPS Task");
		//Delay Task for 2 Seconds
		vTaskDelay(2*configTICK_RATE_HZ);
	}

}


static void vIMUTask(void *pvParameters)
{
	while(1)
	{
	Serial.println("IMU Task");
	//Delay Task for 500ms
	vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
	}
}

static void vAltimeterTask(void *pvParameters)
{
	while(1)
	{
	Serial.println("Altimeter Task");
	//Delay Task for 100ms
	vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);
	}
}

static void vDriverTask(void *pvParameters)
{
	while(1)
	{
	Serial.println("Driver Task");
	//Delay Task for 1s
	vTaskDelay(configTICK_RATE_HZ);
	}
}

static void vTelemetryTask(void *pvParameters)
{
	while(1)
	{
	Serial.println("Telemetry Task");
	//Delay 5 seconds
	vTaskDelay(5*configTICK_RATE_HZ);
	}
}

static void vLoggerTask(void *pvParameters)
{
	while(1)
	{
	Serial.println("Logger Task");
	//Delay 10 seconds
	vTaskDelay(10 * configTICK_RATE_HZ);
	}
}

static void vStateTask (void *pvParameters)
{
	while(1)
	{
	Serial.println("State Machine Task");
	//Delay 4 seconds
	vTaskDelay(4 * configTICK_RATE_HZ);

	}

}
