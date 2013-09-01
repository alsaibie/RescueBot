#include "RR_Motor.h"
#include "RR_CommonDefines.h"


RR_ZoolanderDriver::RR_ZoolanderDriver(void)
{
	
}

void RR_ZoolanderDriver::Enable(void)
{
	servo.Enable();
	motor.Enable();
}

void RR_ZoolanderDriver::Align(void)
{
	//Write Code here to use servo to adjust wheel radius in order to keep moving straight
}

void RR_ZoolanderDriver::Turn(int _degr)
{
	//Take desired change in heading and use motor and servo to turn
}

void RR_ZoolanderDriver::Speed(int _spd)
{
	int _motor_spd, _motor_dir;
	// Do desired calcs for _motor_spd


	//Change motor speed
	motor.Speed(_motor_spd, _motor_dir);
}


RR_ZoolanderDriver::~RR_ZoolanderDriver(void)
{
}
