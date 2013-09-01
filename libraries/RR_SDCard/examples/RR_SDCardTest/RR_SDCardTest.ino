#include <SoftwareSerial.h>
#include <RR_CommonDefines.h>
#include <RR_SDCard.h>

RR_SDCard SD; 
char timemillis[12];
long time1;

void setup() {

	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println("Test Setup");
	SD.Initialize();

	Serial.println(COORDINATE_MSG);
	SD.Write("Hi THIS IS A NIGHT TEST");
	
}

void loop() {
	
	time1=millis();
	SD.WriteMessage(millis()-time1, "$Time");
	Serial.println(millis()-time1);
	
  
}
