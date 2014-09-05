#define ENCODER_OPTIMIZE_INTERRUPTS
#include <RR_Receiver.h>
#include <Encoder.h>
#include <RR_Encoder.h>
#include <RR_CommonDefines.h>
#include <RR_Driver.h>
#include <RR_Motor.h>
#include <RR_CommonData.h>
#include <RR_Telemetry.h>

RR_TelemetryIncomingMessage_t incomingMessages;
RR_TelemetryOutgoingMessage_t outgoingMessage;
RR_NavigationData_t	navdata;

RR_Telemetry radio(&outgoingMessage, &incomingMessages);
RR_Driver ScorpioDriver(&navdata);
RR_Encoder Speedometer;
double lastMillis=0;
void setup()
{  
	Serial.begin(115200);
	ScorpioDriver.Enable();
	delay(2000);
	Serial.println("Telemetry Test");
}

void loop()
{
	radio.Update();
	joystick_t joystick;
	memcpy(&joystick,&incomingMessages.Joystick,sizeof(incomingMessages.Joystick));
	ScorpioDriver.driveManual(joystick, uint16_t(millis()-lastMillis));
	lastMillis=millis();
	Speedometer.Update();
	
	delay(50);
}
