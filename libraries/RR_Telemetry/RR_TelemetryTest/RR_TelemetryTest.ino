#include <RR_CommonData.h>
#include <RR_Telemetry.h>

RR_TelemetryIncomingMessage_t incomingMessages;
RR_TelemetryOutgoingMessage_t outgoingMessage;
RR_Telemetry radio(&outgoingMessage, &incomingMessages);
void setup()
{  
	outgoingMessage.latitude=5;
	outgoingMessage.longitude=200000;
}

void loop()
{
	radio.Update();
	delay(200);
}
