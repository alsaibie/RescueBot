#include "RR_Driver.h"

RR_Driver::RR_Driver(RR_NavigationData_t *data): motors() 
#if USE_RECEIVER
	,receiver()
#else
	,speedometer()
#endif
{

	navigationdata=data;
	leftSpeedOld=0;
	rightSpeedOld=0;
	leftPWMOld=0; rightPWMOld=0;
	for(int k=0; k<3; k++){
		error_left[k]=0;
		error_right[k]=0;
	}
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
#if USE_RECEIVER
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
	
	motors.setSpeeds(rightSpeedOld, leftSpeedOld);
	navigationdata->leftMotorSpeed=leftSpeed;
	navigationdata->rightMotorSpeed=rightSpeed;
#endif
}
void RR_Driver::driveManual(uint16_t SamplingRate)
{
#if USE_RECEIVER
	EffectiveSamplingRate=SamplingRate;
	//Mixing Channels
	int throttle_sig=receiver.getSignal(THRO);
	if(throttle_sig<1100 || throttle_sig>2000) throttle_sig=1500;
	int nominalSpeed=map(throttle_sig,1120,1960,-400,400);
	if(nominalSpeed>-40 && nominalSpeed<40) nominalSpeed=0;

	int turnAngle=map(receiver.getSignal(AILE),1110,1920,-180,180);
	if(DBUG2)
	{
		Serial.print(F("Mapped Throttle:  "));
		Serial.print(throttle_sig); Serial.print(F("  "));
		Serial.println(nominalSpeed);
		//Serial.print(F("Mapped Steer:     "));
		//Serial.print(receiver.getSignal(AILE)); Serial.print(F("  "));
		//Serial.println(turnAngle);
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


	// Apply acceleration limit
	leftSpeedOld=accLimit(leftSpeed,leftSpeedOld, EffectiveSamplingRate);
	rightSpeedOld=accLimit(rightSpeed,rightSpeedOld,EffectiveSamplingRate);
	if(DBUG) {
		Serial.print("Left Speed: ");
		Serial.println(leftSpeedOld);
		Serial.print("Right Speed: ");
		Serial.println(rightSpeedOld);
	}
	//motors.setSpeeds(rightSpeedOld, leftSpeedOld);
	motors.setSpeeds(rightSpeedOld, 0);
	navigationdata->leftMotorSpeed=leftSpeedOld;
	navigationdata->rightMotorSpeed=rightSpeedOld;
	Serial.print("M1 Current:");
	Serial.println(motors.getM1CurrentMilliamps());
	//Serial.print("M2 Current:");
	//Serial.println(motors.getM2CurrentMilliamps());
#endif
}
int16_t RR_Driver::accLimit(int16_t speed, int16_t speedOld, uint16_t samplingRate){

	if(abs(speed-speedOld)>MAXSPEEDCHANGE)
	{
		//speed = speedOld+sgn(speed-speedOld)*MAXSPEEDCHANGE;
	}

	if((abs(speed-speedOld)*1000L)/samplingRate>ACCELERATION_LIMIT)
	{
		int16_t s = speedOld+(sgn(speed-speedOld)*ACCELERATION_LIMIT*samplingRate)/1000L;
		if(s>MOTOR_MAX_SPEED) s=MOTOR_MAX_SPEED;
		if(s<-MOTOR_MAX_SPEED) s=-MOTOR_MAX_SPEED;
		return s;
		//return speed;
	}
	else
	{
		if(speed>MOTOR_MAX_SPEED) speed=MOTOR_MAX_SPEED;
		if(speed<-MOTOR_MAX_SPEED) speed=-MOTOR_MAX_SPEED;
		return speed;
	}
}
void RR_Driver::driveManual(joystick_t data, RR_GPSData_t &gpsdata, RR_IMUData_t &imudata, uint16_t SamplingRate){

	EffectiveSamplingRate=SamplingRate;
	Situation_t Situation;
	//Check Obstacle Situation
	rightCurrent	= motors.getM1CurrentMilliamps();
	leftCurrent	= motors.getM2CurrentMilliamps();
	isObstacled(imudata, gpsdata.isMoving, leftCurrent, rightCurrent, leftActualSpeed, rightActualSpeed, Situation);

	Serial.print("left Current:");
	Serial.println(leftCurrent);
	Serial.print("right Current:");
	Serial.println(rightCurrent);
	int nominalSpeed=map(-data.Pad_Left.Y_Axis,-32768, 32767,-100,100);
	//int nominalSpeed = 50;
	if(nominalSpeed>-5 && nominalSpeed<5) nominalSpeed=0;
	int turnAngle=map(-data.Pad_Right.X_Axis,-32768, 32767,-180,180);
	
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
	setSpeedsFeedback(rightSpeed, leftSpeed);
}

void RR_Driver::driveAutonomous(RR_GPSData_t &gpsdata, RR_IMUData_t &imudata, RR_LoggerData_t &loggerdata, uint16_t SamplingRate)
{
	#if !USE_RECEIVER
	//speedometer.Update();
	//speedometer.getSpeed(&rightActualSpeed,&leftActualSpeed);
	if(0){
	Serial.print("right speed: "); Serial.println(rightActualSpeed);
	Serial.print("left speed: "); Serial.println(leftActualSpeed);}
	#endif
	EffectiveSamplingRate=SamplingRate;
	Situation_t Situation;
	//Check Obstacle Situation
	rightCurrent	= motors.getM1CurrentMilliamps();
	leftCurrent	= motors.getM2CurrentMilliamps();

	isObstacled(imudata, gpsdata.isMoving, leftCurrent, rightCurrent, leftActualSpeed, rightActualSpeed, Situation);

	(Situation==CLEAR? NavigatingState=CRUISING : NavigatingState=OBSTACLED);
	if(DBUG){
		if(Situation==CLEAR){
			Serial.print("Situation: ");
			Serial.println(CLEAR);
		}
	}
	
	//------

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
			wigglingMode(MEDIUM,2);
			break;
		case STALL_RUN:
		case RUN_STALL:
			cruiseModeSimple(getdHeading(imudata, gpsdata));
			break;
	}
	loggerdata.Navigation.speedleft		=	navigationdata->leftMotorSpeed;
	loggerdata.Navigation.speedright	=	navigationdata->rightMotorSpeed;
}

 
void RR_Driver::cruiseModeSimple(int16_t dHeading)
{
	int16_t letfSpeed=0;
	int16_t rightSpeed=0;
	//Basic behavioral steering based on bearing bracket
	if (abs(dHeading)<THRESHOLD_ANGLE_LOWER)
	{
		leftSpeed=SPEED_CRUISE;
		rightSpeed=SPEED_CRUISE;

	}

	else if(abs(dHeading)>THRESHOLD_ANGLE_LOWER && abs(dHeading)<THRESHOLD_ANGLE_UPPER)
	{
		if(dHeading>0)
		{
			leftSpeed=SPEED_CRUISE;
			rightSpeed=SPEED_MANEUVER;
		}
		else
		{
			leftSpeed=SPEED_MANEUVER;
			rightSpeed=SPEED_CRUISE;
		}
	}

	else if(abs(dHeading)>THRESHOLD_ANGLE_UPPER)
	{
		if(dHeading>0)
		{
			leftSpeed=SPEED_LOW;
			rightSpeed=-SPEED_LOW;
		}
		else
		{
			leftSpeed=-SPEED_LOW;
			rightSpeed=SPEED_LOW;
		}
	}

	if(0){
		Serial.print("Left Speed: ");
		Serial.print(leftSpeedOld);
		Serial.print(", Right: ");
		Serial.print(rightSpeedOld);
		Serial.print(", dHeading: ");
		Serial.println(dHeading);
	}
	setSpeedsFeedback(rightSpeed, leftSpeed);
	
}
void RR_Driver::cruiseModeAdvanced(int8_t dHeading)
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
	setSpeedsFeedback(rightSpeed, leftSpeed);
}

void RR_Driver::tipoverMode(RR_IMUData_t &imudata)
{
	//Timed Rotation or While its sideways
	//For now just do a timed rotation routine
	//but should incorporate imu so that routine will continue until
	//robot is not stuck anymore - need noiseless accelerometer for that
	motors.setSpeeds(400,-400);
	delay(5000);
	motors.setSpeeds(0,0);
	delay(1000);
	motors.setSpeeds(-400,400);
	delay(5000);
	motors.setSpeeds(0,0);
	delay(1000);

}
void RR_Driver::setFree(void){
	motors.setSpeeds(SPEED_LOW,SPEED_LOW);
	delay(50000);
	motors.setSpeeds(0,0);
}

void RR_Driver::wigglingMode(Speed_t speedlevel, uint8_t repeats)
{
	//Timed Reverse CW and CCW
	for(uint8_t k = 0; k < repeats; k++){
	motors.setSpeeds(400,-400);
	delay(3000);
	motors.setSpeeds(0,0);
	delay(1000);
	motors.setSpeeds(-400,400);
	delay(3000);
	motors.setSpeeds(0,0);
	delay(1000);
	}
}

void RR_Driver::reciprocatingMode(Speed_t speedlevel)
{
	//Timed Back and Forth
	motors.setSpeeds(400,400);
	delay(3000);
	motors.setSpeeds(0,0);
	delay(1000);
	motors.setSpeeds(-400,-400);
	delay(3000);
	motors.setSpeeds(0,0);
}

int16_t RR_Driver::getdHeading(RR_IMUData_t &imudata, RR_GPSData_t &gpsdata)
{
	int16_t dHeading=uint16_t(gpsdata.Bearing+imudata.headingFiltered)%360;
	dHeading = (dHeading>180? dHeading-360: dHeading);

	if(DBUG)
	{
		Serial.print("Heading: ");
		Serial.print(imudata.headingFiltered);
		Serial.print(", Delta: ");
		Serial.println(dHeading);
	}
	return dHeading;
}
void RR_Driver::isObstacled(RR_IMUData_t &imudata, bool isMoving, uint16_t left_current, uint16_t right_current, uint16_t left_speed, uint16_t right_speed, Situation_t &situation)
{
		//Situation = something, either CLEAR or 
	if(DBUG2){
		Serial.print("is Moving: "); Serial.println(isMoving);
		Serial.print("leftCurrent: "); Serial.println(left_current);
		Serial.print("rightCurrent: "); Serial.println(right_current);
		Serial.print("actualleftRPM: "); Serial.println(left_speed);
		Serial.print("actualrightRPM: "); Serial.println(right_speed);
		Serial.print("Y Acc: "); Serial.println(imudata.accelerometer.y);
	}
	if(!isMoving){
		if(imudata.accelerometer.y>8){
			//tipped
		}
		else if(imudata.accelerometer.y<-8){
			//tipped other side
		}
		else if(0){
	
		}
		else if(0){

		}
		else if(0){

		}
	}

	else {
		situation = CLEAR;
	}

	if(DBUG2){
		Serial.print("Situation: "); Serial.println(situation);
	}



}	
void RR_Driver::setSpeedsFeedback(int16_t rightSpeed, int16_t leftSpeed){
	#if !USE_RECEIVER
	speedometer.Update();
	speedometer.getSpeed(&rightActualSpeed,&leftActualSpeed);
	if(DBUG3){
	Serial.print("right speed: "); Serial.print(rightActualSpeed);
	Serial.print(", d: "); Serial.println(rightSpeed*MOTOR_MAX_SPEED/100);
	Serial.print("left speed: "); Serial.print(leftActualSpeed);
	Serial.print(", d: "); Serial.println(leftSpeed*MOTOR_MAX_SPEED/100);}
	#endif
	error_left[2]=error_left[1];
	error_left[1]=error_left[0];
	error_left[0]=(leftSpeed*MOTOR_MAX_SPEED/100 - leftActualSpeed);
	integral_left+=error_left[0];
	//leftPWM = (leftSpeed*Kt*.93 + (Kp100 +Ki100 +Kd100) * error_left[0] + (-Kp100 -2*Kd100) *error_left[1] + Kd100 *error_left[2])/100; //Feedback Error
	leftPWM = (Kp100  * error_left[1] + Ki100  * integral_left + Kd100 *(error_left[0]-error_left[1]))/100; //Feedback Error
	leftPWMOld = accLimit(leftPWM,leftPWMOld,EffectiveSamplingRate);

	error_right[2]=error_right[1];
	error_right[1]=error_right[0];
	error_right[0]=((rightSpeed*MOTOR_MAX_SPEED)/100 - rightActualSpeed);
	integral_right+=error_right[0];
	//rightPWM = (rightSpeed*Kt + (Kp100 +Ki100 +Kd100) * error_right[0] + (-Kp100 -2*Kd100) *error_right[1] + Kd100 *error_right[2])/100; //Feedback Error
	rightPWM = (Kp100  * error_right[1] + Ki100  * integral_right + Kd100 *(error_right[0]-error_right[1]))/100; //Feedback Error
	rightPWMOld = accLimit(rightPWM,rightPWMOld,EffectiveSamplingRate);
	
	//leftSpeedOld=accLimit(leftSpeed,leftSpeedOld, EffectiveSamplingRate);
	//rightSpeedOld=accLimit(rightSpeed,rightSpeedOld,EffectiveSamplingRate);
		if(DBUG3){
			
	Serial.print("right PWM: "); Serial.println(rightPWMOld); 
	Serial.print("left PWM: "); Serial.println(leftPWMOld);}
	//motors.setSpeeds(rightPWMOld,leftPWMOld);
		motors.setSpeeds((rightSpeed*MOTOR_MAX_SPEED)/100,(leftSpeed*MOTOR_MAX_SPEED)/100);
	//motors.setSpeeds(accLimit(rightSpeed,rightSpeedOld,EffectiveSamplingRate),
	//	accLimit(leftSpeed*.92,leftSpeedOld,EffectiveSamplingRate));
	//rightSpeedOld=rightSpeed;
	//leftSpeedOld=leftSpeed*.92;
	//motors.setSpeeds(rightPWMOld,0);
	navigationdata->leftMotorSpeed=leftActualSpeed;
	navigationdata->rightMotorSpeed=rightActualSpeed;
}
void RR_Driver::getMotorData(int16_t *leftspeed, int16_t *rightspeed, uint16_t *leftcurrent, uint16_t *rightcurrent){
	*leftspeed=leftActualSpeed;
	*rightspeed=rightActualSpeed;
	*leftcurrent=leftCurrent;
	*rightcurrent=rightCurrent;
}

RR_Driver::~RR_Driver(void)
{
}
