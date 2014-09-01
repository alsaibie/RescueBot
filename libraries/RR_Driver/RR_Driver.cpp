#include "RR_Driver.h"

RR_Driver::RR_Driver(RR_NavigationData_t *data):receiver(), motors(), speedometer()
{
	navigationdata=data;
}

void RR_Driver::Enable(void)
{
	motors.init();

}

void RR_Driver::Stop(void)
{
	motors.setM1Speed(0);
	motors.setM2Speed(0);
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
		if(0)
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
		if(0)
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
	navigationdata->leftMotorSpeed=leftSpeed;
	navigationdata->rightMotorSpeed=rightSpeed;

}

void RR_Driver::driveManual(joystick_t data){
	leftSpeed=-map(data.Pad_Left.Y_Axis,-32768, 32767,-400,400);
	rightSpeed=-map(data.Pad_Right.Y_Axis,-32768, 32767,-400,400);
	motors.setM1Speed(leftSpeed);
	motors.setM2Speed(rightSpeed);
	navigationdata->leftMotorSpeed=leftSpeed;
	navigationdata->rightMotorSpeed=rightSpeed;
}

void RR_Driver::driveAutonomous(RR_GPSData_t &gpsdata, RR_IMUData_t &imudata, RR_LoggerData_t &loggerdata)
{
	Situation_t Situation;
	isObstacled(imudata, Situation);

	if(Situation==CLEAR? NavigatingState=CRUISING : NavigatingState=OBSTACLED)
		Serial.print("Situation: ");
		Serial.println(CLEAR);
	switch(Situation)
	{
		case CLEAR:
			cruiseModeSimple(getdHeading(imudata, gpsdata));
			break;
		case TIPPED:
			tipoverMode(imudata);
			break;
		case STALL_STALL:
		case STALL_FREE:
		case FREE_STALL:
			reciprocatingMode();
			break;
		case FREE_FREE:
		case FREE_RUN:
		case RUN_FREE:
			wigglingMode();
			break;
		case STALL_RUN:
		case RUN_STALL:
			cruiseModeSimple(getdHeading(imudata, gpsdata));
			break;
	}
}

 
void RR_Driver::cruiseModeSimple(uint8_t dHeading)
{
	//Basic behavioral steering based on bearing bracket
	if (abs(dHeading)<THRESHOLD_ANGLE_LOWER)
	{
		motors.setSpeeds(SPEED_CRUISE, SPEED_CRUISE);
	}

	else if(abs(dHeading)>THRESHOLD_ANGLE_LOWER && abs(dHeading)<THRESHOLD_ANGLE_UPPER)
	{
		if(dHeading>0)
		{
			motors.setSpeeds(SPEED_MANEUVER,SPEED_CRUISE);
		}
		else
		{
			motors.setSpeeds(SPEED_MAX,SPEED_MANEUVER);
		}
	}

	else if(abs(dHeading)>THRESHOLD_ANGLE_UPPER)
	{
		if(dHeading>0)
		{
			motors.setSpeeds(-SPEED_LOW,SPEED_LOW);
		}
		else
		{
			motors.setSpeeds(SPEED_LOW,-SPEED_LOW);
		}
	}
}
void RR_Driver::cruiseModeAdvanced(uint8_t dHeading)
{
	//If Compass Data is Reliable just have an efficient feedback steering logic
	if(abs(dHeading)<10)
	{
		leftSpeed=SPEED_CRUISE;
		rightSpeed=SPEED_CRUISE;
	}
	else
	{
		//PD Controller for Steering
	}
	motors.setSpeeds(leftSpeed,rightSpeed);
}

void RR_Driver::tipoverMode(RR_IMUData_t &imudata)
{
	//Timed Rotation or While its sideways
}

void RR_Driver::wigglingMode(Speed_t speedlevel)
{
	//Timed Reverse CW and CCW
}

void RR_Driver::reciprocatingMode(Speed_t speedlevel)
{
	//Timed Back and Forth
}

int8_t RR_Driver::getdHeading(RR_IMUData_t &imudata, RR_GPSData_t &gpsdata)
{
	int dHeading=gpsdata.Bearing-imudata.fused.heading;

	if(DBUG)
	{
		Serial.print("Delta Heading:  ");
		Serial.print(dHeading);
		Serial.println(" deg");
	}

	return dHeading;
}

void RR_Driver::isObstacled(RR_IMUData_t &imudata, Situation_t &Situation)
{}	

RR_Driver::~RR_Driver(void)
{
}
