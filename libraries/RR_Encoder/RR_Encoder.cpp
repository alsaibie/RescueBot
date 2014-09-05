#include "RR_Encoder.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_Encoder::RR_Encoder(void): ENCA(ENCA1_PIN,ENCA2_PIN), ENCB(ENCB1_PIN,ENCB2_PIN)
{

	oldPos1=-999;
	oldPos2=-999;
	lastMillis1=0;
	lastMillis2=0;
}

void RR_Encoder::Update(void)
{
	
	newPos1 = ENCA.read();
	//Serial.println(ENCA.read());
	if(0)
	{
		if (newPos1 != oldPos1) 
		{
		Serial.print("Revolution A:  ");
		Serial.println(-((float)newPos1/(MOTOR_GEAR_RATIO*ENCODER_COUNT)));
		}
	}
	speed1=(60*(newPos1-oldPos1))/(millis()-lastMillis1);
	lastMillis1=millis();
	oldPos1 = newPos1;
	if(0)
	{
		Serial.print("Speed A:  ");
		Serial.println(speed1);
	}

	newPos2 = ENCB.read();

	if(0)
	{
		if (newPos2 != oldPos2) 
		{
		Serial.print("Revolution B:  ");
		Serial.println(((float)newPos2/(MOTOR_GEAR_RATIO*ENCODER_COUNT)));
		}
	}
	speed2=-(60*(newPos2-oldPos2))/(millis()-lastMillis2);

	lastMillis2=millis();
	oldPos2 = newPos2;
	if(0)
	{
		Serial.print("Speed B:  ");
		Serial.println(speed2);
	}

}

void RR_Encoder::getSpeed(int16_t *mot1, int16_t *mot2)
{
	*mot1=speed1;
	*mot2=speed2;
}

RR_Encoder::~RR_Encoder(void)
{
}
