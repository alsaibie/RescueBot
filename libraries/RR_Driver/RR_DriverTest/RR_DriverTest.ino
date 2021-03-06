#include <Encoder.h>
#include <RR_Encoder.h>
#include <RR_CommonData.h>
#include <RR_Receiver.h>
#include <RR_Motor.h>
#include <RR_Driver.h>
#include <RR_CommonDefines.h>

RR_NavigationData_t	navdata;
RR_Driver ScorpioDriver(&navdata);
double lastMillis=0;
void setup()
{
	Serial.begin(115200);
	delay(3000);
	Serial.println("Driver Test");
	ScorpioDriver.Enable();
	
	
}

void loop()
{
	ScorpioDriver.driveManual(uint16_t(millis()-lastMillis));

	lastMillis=millis();
	delay(500);
}

