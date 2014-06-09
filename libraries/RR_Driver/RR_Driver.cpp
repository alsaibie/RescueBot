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
	int nominalSpeed=map(receiver.getSignal(THRO),1120,1970,-400,400);
	int turnAngle=map(receiver.getSignal(AILE),1110,1920,-180,180);
	if(DBUG)
	{
		Serial.print(F("Mapped Throttle:  "));
		Serial.print(receiver.getSignal(THRO)); Serial.print(F("  "));
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
