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
xTaskHandle _htaskGPS, _htaskIMU, _htaskAltiemter,  _htaskDriver, _htaskTelemetry, _htaskLogger;
//Queue Handles
xQueueHandle _queuehandle1;
//Queues to create: Need to think about the message passing structure first.
//Semaphore Handles
xSemaphoreHandle _semaphorehandle1;

static void vGPSTask(void *pvParameters);
static void vIMUTask(void *pvParameters);
static void vAltimeterTask(void *pvParameters);
static void vDriverTask(void *pvParameters);
static void vTelemetryTask(void *pvParameters);
static void vLoggerTask(void *pvParameters);


void setup()
{

xTaskCreate(vGPSTask,		(signed portCHAR *)"GPS Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskGPS);
xTaskCreate(vIMUTask,		(signed portCHAR *)"IMU Task",			configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskIMU);
xTaskCreate(vAltimeterTask, (signed portCHAR *)"Altimeter Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskAltiemter);
xTaskCreate(vDriverTask,	(signed portCHAR *)"Driver Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskDriver);
xTaskCreate(vTelemetryTask, (signed portCHAR *)"Telemetry Task",	configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskTelemetry);
xTaskCreate(vLoggerTask,	(signed portCHAR *)"Logger Task",		configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 2, &_htaskLogger);

}

void loop()
{

  /* Nothing to see here */

}

static void vGPSTask(void *pvParameters)
{

}

static void vIMUTask(void *pvParameters)
{

}

static void vAltimeterTask(void *pvParameters)
{

}

static void vDriverTask(void *pvParameters)
{

}

static void vTelemetryTask(void *pvParameters)
{

}

static void vLoggerTask(void *pvParameters)
{

}
