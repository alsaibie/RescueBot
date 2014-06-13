#include "RR_Driver.h"

RR_Driver::RR_Driver(void):receiver(), motors()
{
}

void RR_Driver::Enable(void)
{
	motors.init();
}


void RR_Driver::driveManual(void)
{
	//Mixing Channels
	int throttle_sig=receiver.getSignal(THRO);
	if(throttle_sig<1100 || throttle_sig>2000) throttle_sig=1500;
	int nominalSpeed=map(throttle_sig,1120,1960,-400,400);
	if(nominalSpeed>-40 && nominalSpeed<40) nominalSpeed=0;

	int turnAngle=map(receiver.getSignal(AILE),1110,1920,-180,180);
	if(DBUG)
	{
		Serial.print(F("Mapped Throttle:  "));
		Serial.print(throttle_sig); Serial.print(F("  "));
		Serial.println(nominalSpeed);
		Serial.print(F("Mapped Steer:     "));
		Serial.print(receiver.getSignal(AILE)); Serial.print(F("  "));
		Serial.println(turnAngle);
	}

	if(turnAngle>20) //Turn Left
	{
		rightSpeed=nominalSpeed;
		leftSpeed=(nominalSpeed*(1-(float)turnAngle/90));
		if(DBUG)
			{
				Serial.print(F("Mapped leftspeed:  "));
				Serial.print(nominalSpeed); Serial.print(F("  "));
				Serial.println(leftSpeed);
			}

	}
	
	else if(turnAngle<-20) //Turn Left
	{
		leftSpeed=nominalSpeed;
		rightSpeed=(nominalSpeed*(1+(float)turnAngle/90));
		if(DBUG)
			{
				Serial.print(F("Mapped rightspeed:  "));
				Serial.print(nominalSpeed); Serial.print(F("  "));
				Serial.println(rightSpeed);
			}

	}
	else
	{
		leftSpeed=nominalSpeed;
		rightSpeed=nominalSpeed;
	}

	motors.setM1Speed(leftSpeed);
	motors.setM2Speed(rightSpeed);

}

//void RR_Driver::driveAuto(void)
//{

//}

RR_Driver::~RR_Driver(void)
{
}
