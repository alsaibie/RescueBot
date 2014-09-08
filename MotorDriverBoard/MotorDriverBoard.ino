
#include <RR_CommonDefines.h>
#include <RR_CommonData.h>
#include <RR_Receiver.h>
#include <RR_Motor.h>
#include <RR_Driver.h>
//RECEIVER
#undef THRO_RX_PIN
#undef AILE_RX_PIN

//RECEIVER
#define THRO_RX_PIN 5
#define AILE_RX_PIN 11
RR_NavigationData_t	navdata;
RR_Driver Driver(&navdata);
double lastMillis=0;
void setup()
{
	Serial.begin(115200);
	delay(3000);
	Serial.println("Driver Test");
	Driver.Enable();
	
	
}

void loop()
{
	Driver.driveManual(uint16_t(millis()-lastMillis));
	lastMillis=millis();
	delay(100);
}
