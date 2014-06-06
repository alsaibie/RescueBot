#include <RR_Driver.h>
#include <RR_CommonDefines.h>

RR_Receiver Spektrum;
RR_Driver ScorpioDriver(&Spektrum);

void setup()
{
	Serial.begin(115200);
	delay(3000);
	Serial.println("Driver Test");
}

void loop()
{
	ScorpioDriver.driveManual();
}

