#include "RR_SDCard.h"


RR_SDCard::RR_SDCard(RR_LoggerData_t *data)
{
	loggerData=data;
}

//This function creates a given file and then opens it in append mode (ready to record characters to the file)
//Then returns to listening mode
void RR_SDCard::Initialize(void)
{
	//Serial.println("Test Constructor");
	Serial2.begin(9600);
	//Reset the processor on the Open Log and wait for Open Log to be ready
	//Reset OpenLog
	pinMode(OPENLOG_GRN_PIN, OUTPUT);
	delay(200);
	digitalWrite(OPENLOG_GRN_PIN, LOW);
	delay(100);
	digitalWrite(OPENLOG_GRN_PIN, HIGH);
	
  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while(1) {
	if(Serial2.available())
	  if(Serial2.read() == '<') break;
  }
 

	char fileName[12]; //Max file name length is "12345678.123" (12 characters)
	sprintf(fileName, "RBot%04d.txt", 2014);

  //Send three control z to enter OpenLog command mode
	Serial2.write(26);
	Serial2.write(26);
	Serial2.write(26);
	
	//Wait for OpenLog to respond with '>' to indicate we are in command mode
	while(1) {
		if(Serial2.available())
			if(Serial2.read() == '>') break;
			 }
    
	Serial2.print("new ");
	Serial2.print(fileName);
	Serial2.write(13); //This is \r
	//Wait for OpenLog to return to waiting for a command
	while(1) {
	 if(Serial2.available())
		  if(Serial2.read() == '>') break;
			 }
	
	Serial2.print("append ");
	Serial2.print(fileName);
	Serial2.write(13); //This is \r
 
	
  //Wait for OpenLog to indicate file is open and ready for writing
	while(1) {
		//if(DBUG){Serial.println("Wait to indicate open and ready for writing");}
	 if(Serial2.available())
		  if(Serial2.read() == '<') break;
			}

  //OpenLog is now waiting for characters and will record them to the new file  
	Serial2.println("/////////////////////NEWFILE////////////////////////");
	if(DBUG) Serial.println("Init Complete");

}


void RR_SDCard::Write(const char *_str)
{
	Serial2.println(_str);
}

void RR_SDCard::WriteMessage(float _msg_value, const char  *_msg_type)
{
	Serial2.print(_msg_type);
	Serial2.println(_msg_value,3);
}


void RR_SDCard::updateLog(void)
{
	//Log GPS Data	
	Serial2.print(GPS_LOG_MSG); comma;
	Serial2.print((uint32_t)(millis()/100)); comma;
	if(loggerData->GPS.fix){
		Serial2.print("Y"); comma;
		Serial2.print(loggerData->GPSTime.Hour); Serial2.print(loggerData->GPSTime.Minute); 
		Serial2.print(loggerData->GPSTime.Seconds); comma;
		Serial2.print(loggerData->GPS.Latitude,3); comma;
		Serial2.print(loggerData->GPS.Lat); comma;
		Serial2.print(loggerData->GPS.Longitude,3); comma;
		Serial2.print(loggerData->GPS.Lon); comma;
		Serial2.print(loggerData->GPS.DistanceToTarget); comma;
		Serial2.print(loggerData->GPS.Bearing); comma;
	}
	else{
		Serial2.print("N"); comma;
	}
	hashEnd;
	//---------------

	// Log State Data
	Serial2.print(STATE_LOG_MSG); comma;
	Serial2.print((uint32_t)(millis()/100)); comma;
	Serial2.print(loggerData->State.mainstate); comma;
	if(loggerData->State.mainstate==NAVIGATING){
	Serial2.print(loggerData->State.navstate); comma;
	}
	hashEnd;
	//---------------

	//Log Navigation Data
	if(loggerData->State.mainstate==NAVIGATING){
	Serial2.print(NAVIGATION_LOG_MSG); comma;
	Serial2.print((uint32_t)(millis()/100)); comma;
	Serial2.print(loggerData->Navigation.speed); comma;
	hashEnd;
	}
	//---------------

	//Log Altitude Data
	if(loggerData->State.mainstate<NAVIGATING){
	Serial2.print(ALTITUDE_LOG_MSG); comma;
	Serial2.print((uint32_t)(millis()/100)); comma;
	Serial2.print(loggerData->Altitude); comma;
	hashEnd;
	}
	//---------------
}

RR_SDCard::~RR_SDCard(void)
{}