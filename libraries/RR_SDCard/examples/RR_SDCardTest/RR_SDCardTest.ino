#include <RR_CommonDefines.h>
#include <RR_SDCard.h>

RR_SDCard SD(&Serial2); 
char timemillis[12];
long time1;

void setup() {
	delay(4000);
	Serial.begin(56700);
	Serial.println("Test Setup");
	SD.Initialize();
	delay(2000);
	Serial.println(COORDINATE_MSG);
	SD.Write("Hi THIS IS A NIGHT TEST");
	
}

void loop() {
	
	time1=millis();
	SD.WriteMessage(millis()-time1, "$Time");
	Serial.println(millis()-time1);
	
  
}
