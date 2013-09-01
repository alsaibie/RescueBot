#include "RR_SDCard.h"


RR_SDCard::RR_SDCard(void):SD_Serial(OPENLOG_RX_PIN,OPENLOG_TX_PIN)
{

}

//This function creates a given file and then opens it in append mode (ready to record characters to the file)
//Then returns to listening mode
void RR_SDCard::Initialize(void)
{
	//Serial.println("Test Constructor");
	SD_Serial.begin(9600);
	//Reset the processor on the Open Log and wait for Open Log to be ready
	//Reset OpenLog
	pinMode(OPENLOG_GRN_PIN, OUTPUT);
	delay(200);
	digitalWrite(OPENLOG_GRN_PIN, LOW);
	//delay(50);
	digitalWrite(OPENLOG_GRN_PIN, HIGH);
	
  //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  while(1) {
	  Serial.println("Wait");
	if(SD_Serial.available())
	  if(SD_Serial.read() == '<') break;
  }
 

	char fileName[12]; //Max file name length is "12345678.123" (12 characters)
	sprintf(fileName, "RBot%04d.txt", 2013);

  //Send three control z to enter OpenLog command mode
	SD_Serial.write(26);
	SD_Serial.write(26);
	SD_Serial.write(26);
	
	//Wait for OpenLog to respond with '>' to indicate we are in command mode
	while(1) {
		if(SD_Serial.available())
			if(SD_Serial.read() == '>') break;
			 }
    
	SD_Serial.print("new ");
	SD_Serial.print(fileName);
	SD_Serial.write(13); //This is \r
	Serial.println("Broke");
  //Wait for OpenLog to return to waiting for a command
	while(1) {
	 if(SD_Serial.available())
		  if(SD_Serial.read() == '>') break;
			 }
	
	SD_Serial.print("append ");
	SD_Serial.print(fileName);
	SD_Serial.write(13); //This is \r
 
	
  //Wait for OpenLog to indicate file is open and ready for writing
	while(1) {
	 if(SD_Serial.available())
		  if(SD_Serial.read() == '<') break;
			}

  //OpenLog is now waiting for characters and will record them to the new file  
	SD_Serial.println("/////////////////////////////////////////////");

}


void RR_SDCard::Write(const char *_str)
{
	SD_Serial.println(_str);
}

void RR_SDCard::WriteMessage(float _msg_value, const char  *_msg_type)
{
	SD_Serial.print(_msg_type);
	SD_Serial.println(_msg_value,3);
}

RR_SDCard::~RR_SDCard(void)
{
}
