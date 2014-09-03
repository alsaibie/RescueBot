#include <RR_CommonData.h>
#include <RR_CommonDefines.h>
#include <RR_SDCard.h>

RR_LoggerData_t loggerData;
RR_SDCard SD(&loggerData);

char timemillis[12];
long time1;

void setup() {
	delay(4000);
	Serial.begin(56700);
	Serial.println("SD Card Test");
	SD.Initialize();
	delay(2000);
	loggerData.GPS.fix=true;
	loggerData.GPS.Latitude=3347.342;

	loggerData.GPS.Longitude=8424.003;

	loggerData.GPSTime.Hour=12;
	loggerData.GPSTime.Minute=50;
	loggerData.GPSTime.Seconds=15;
	loggerData.State.mainstate=NAVIGATING;
	loggerData.State.navstate=CRUISING;
	loggerData.Navigation.speed=230;
	Serial.println("Setup Complete");
}

void loop() {
	uint32_t time1=millis();
	SD.updateLog();
	Serial.println(millis()-time1);  
	delay(1000);
}
