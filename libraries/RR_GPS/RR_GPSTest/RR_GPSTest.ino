#include <RR_CommonData.h>
#include <FreeRTOS_ARM.h>
#include <basic_io_arm.h>
#include <RR_CommonDefines.h>
#include <Adafruit_GPS.h>
#include <RR_GPS.h>
#undef F
#define F(str) str

RR_GPSData_t gpsData;
RR_GPS gps(&gpsData);

//Task Handles
xTaskHandle gpsTask;

//Mutex
xSemaphoreHandle gpsDataMutex, SerialSemaphore;

static void vGPSTask(void *pvParameters);

void setup()
{  
	Serial.begin(115200);
	gps.Enable();
	xTaskCreate(vGPSTask,(signed portCHAR *)"GPS Task", configMINIMAL_STACK_SIZE + 100, NULL, tskIDLE_PRIORITY + 2, &gpsTask);
	vSemaphoreCreateBinary(gpsDataMutex);
	// start FreeRTOS
	vTaskStartScheduler();
	
	// should never return
  Serial.println(F("Die"));
  while(1);
}

void loop()
{
	delay(3000);
}

static void vGPSTask(void *pvParameters)
{
	while(1)
	{
	gps.Update();
	if(gps.newGPSData)
	{
		if(gps.fix)
		{
		xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
		//Update
			gps.getData();
			Serial.println(gpsData.Time.Seconds);
			xSemaphoreGive(gpsDataMutex);
		}
		else
		{
			//if(DBUG)
				//Serial.println("NoFix");
		}

	}
	vTaskDelay((1L *configTICK_RATE_HZ)/1000L);
	}
}
