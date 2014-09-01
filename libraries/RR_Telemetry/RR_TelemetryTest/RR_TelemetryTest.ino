#include <RR_CommonData.h>
#include <RR_Telemetry.h>

RR_TelemetryIncomingMessage_t incomingMessages;
RR_TelemetryOutgoingMessage_t outgoingMessage;
RR_Telemetry radio(&outgoingMessage, &incomingMessages);
void setup()
{  
	outgoingMessage.Date.Year=2014;
	outgoingMessage.Date.Month=06;
	outgoingMessage.Date.Day=29;
	Serial.begin(115200);
	delay(2000);
	Serial.println("Telemetry Test");
	//radio.SendSettingCommand("ATS4=10");
/*	Serial3.write("+");
	Serial3.write("+");
	Serial3.write("+");
	delay(150);
	if(DBUG){
		while(Serial3.available()){
			Serial.println(Serial3.read());
		}
	}
	delay(1500);
		if(DBUG){
		while(Serial3.available()){
			Serial.println(Serial3.read());
		}
	}

	
	
	Serial3.print("ATS4?");
	delay(150);
	if(DBUG){
		while(Serial3.available()){
			Serial.print(Serial3.read());
		}
	}
	delay(1000);
	Serial3.print("ATO");
	*/

}

void loop()
{
	radio.Update();
	delay(50);
}
