// Simple demo of three threads
// LED blink thread, print thread, and idle loop
#include <FreeRTOS_ARM.h>
// Redefine AVR Flash string macro as nop for ARM
#undef F
#define F(str) str
#define LED_ITEM_SIZE           sizeof(uint8_t)
#define LED_QUEUE_SIZE          1

const uint8_t LED_PIN = 13;

volatile uint32_t count = 0;

// handle for blink task
xTaskHandle blink;
// handle for LEDFlash messages sent to printtask
xQueueHandle LEDQueue;
//Semaphore for protecting access to Serial (UART)
xSemaphoreHandle SerialSemaphore;

//------------------------------------------------------------------------------
// high priority for blinking LED
static void vLEDFlashTask(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
  uint8_t ui8Message=0;
  // Flash led every 200 ms.
  for (;;) {
	  if(xQueueReceive(LEDQueue, &ui8Message, xTicksToWait ) == pdPASS )
		   {
				// Turn LED on.
				digitalWrite(LED_PIN, HIGH);
				Serial.println(ui8Message);
				// Sleep for 50 milliseconds.
				vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
			
				// Turn LED off.
				digitalWrite(LED_PIN, LOW);
    
				// Sleep for 150 milliseconds.
				//vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
			}
  }
}
//------------------------------------------------------------------------------
static void vPrintTask(void *pvParameters) {
	uint8_t ui8Message=0;
	
	if( LEDQueue == 0 )
	{
		Serial.print("Queue was not created \n");
	}
	
  while (1) {
    // Sleep for one second.
    vTaskDelay(configTICK_RATE_HZ);
    ui8Message++;
	if(xQueueSend(LEDQueue, &ui8Message, 0 ) != pdTRUE)
    {
        Serial.print("\nQueue full. This should never happen.\n");
        while(1){}
    }
	
   // delay(2000);
    // Zero count.
    count = 0;
  }
}
//------------------------------------------------------------------------------
void setup() {
	
  Serial.begin(9600);
  LEDQueue = xQueueCreate(LED_QUEUE_SIZE, LED_ITEM_SIZE);
  // wait for Leonardo
  while(!Serial) {}
  
  // create blink task
  xTaskCreate(vLEDFlashTask,
    (signed portCHAR *)"Task1",
    configMINIMAL_STACK_SIZE + 50,
    NULL,
    tskIDLE_PRIORITY + 2,
    &blink);
  
  // create print task
  xTaskCreate(vPrintTask,
    (signed portCHAR *)"Task2",
    configMINIMAL_STACK_SIZE + 100,
    NULL,
    tskIDLE_PRIORITY + 1,
    NULL);

  // start FreeRTOS
  vTaskStartScheduler();
  
  // should never return
  Serial.println(F("Die"));
  while(1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {

}
