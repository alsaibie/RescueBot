#include "RR_Encoder.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_Encoder::RR_Encoder(void): ENCB(ENCB1_PIN,ENCB2_PIN)//, ENCB(ENCB1_PIN,ENCB2_PIN)
{
	oldPos1=-999;
	oldPos2=-999;
}

void RR_Encoder::Update(void)
{
	/*
	newPos1 = ENCA.read();
	if(DBUG)
	{
		if (newPos1 != oldPos1) 
		{
			oldPos1 = newPos1;
			Serial.print("Revolution A:  ");
			Serial.println(-((float)newPos1/(MOTOR_GEAR_RATIO*ENCODER_COUNT)));
		}
	}
	*/

	newPos2 = ENCB.read();

	if(DBUG)
	{

		if (newPos2 != oldPos2) 
		{
			oldPos2 = newPos2;
			Serial.print("Revolution B:  ");
			Serial.println(-((float)newPos2/(MOTOR_GEAR_RATIO*ENCODER_COUNT)));
		}
	}

}

void RR_Encoder::getSpeed(int *mot1, int *mot2)
{

}

RR_Encoder::~RR_Encoder(void)
{
}
