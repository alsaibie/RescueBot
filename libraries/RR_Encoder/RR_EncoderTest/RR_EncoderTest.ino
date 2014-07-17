#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <RR_Motor.h>
#include <RR_CommonDefines.h>
#include <RR_Encoder.h>

RR_Motor Motors;
RR_Encoder Speedometer;
void setup()
{ 
	pinMode(THRO_RX_PIN,INPUT);
	pinMode(AILE_RX_PIN,INPUT);
	//pinMode(ENCB1_PIN,INPUT);
	//pinMode(ENCB2_PIN,INPUT);
	Motors.init();
	Serial.begin(115200);
}

void loop()
{
	int throttle =	pulseIn(THRO_RX_PIN, HIGH, 25000);
	int speed =		map(throttle, 1100,1900,-400,400);

	if(speed <15 && speed>-15)
	{
		speed = 0;
	}

	if(speed==-1500)
	{
		speed = 0;
	}
			//Serial.println(speed);

	Motors.setM1Speed(speed);
	Speedometer.Update();
	/*
	long newPosition = myEnc.read();
    if (newPosition != oldPosition) {
    oldPosition = newPosition;
	Serial.print("Revolution:  ");
	Serial.println(-((float)newPosition/(MOTOR_GEAR_RATIO*ENCODER_COUNT)));
  }
  */
	delay(100);

}
