#include "RR_SpheroDriver.h"

RR_SpheroDriver::RR_SpheroDriver(void)
{

}

void RR_SpheroDriver::Enable(void)
{
	servo.Enable();
	motor.Enable();
}

void RR_SpheroDriver::Turn(int _degr)
{
	//Take desired change in heading and use motor and servo to turn
}

void RR_SpheroDriver::Speed(int _spd)
{
	int _motor_spd, _motor_dir;
	// Do desired calcs for _motor_spd


	//Change motor speed
	motor.Speed(_motor_spd, _motor_dir);
}

void RR_SpheroDriver::Align(void)
{
	//Write Code here to use servo to adjust wheel radius in order to keep moving straight
}


RR_SpheroDriver::~RR_SpheroDriver(void)
{
}
