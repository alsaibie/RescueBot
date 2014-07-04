#include <RR_CommonData.h>
#include <RR_Telemetry.h>

RR_TelemetryIncomingMessage_t incomingMessages;
RR_TelemetryOutgoingMessage_t outgoingMessage;
RR_Telemetry radio(&outgoingMessage, &incomingMessages);
void setup()
{  
	outgoingMessage.latitude=5;
	outgoingMessage.longitude=200000;
	outgoingMessage.Date.Year=2014;
	outgoingMessage.Date.Month=06;
	outgoingMessage.Date.Day=29;
}

void loop()
{
	Serial.begin(56700);
	radio.Update();
	delay(200);
}
