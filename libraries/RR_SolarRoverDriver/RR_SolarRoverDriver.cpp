#include "RR_SolarRoverDriver.h"

RR_SolarRoverDriver::RR_SolarRoverDriver(void)
{

}

void RR_SolarRoverDriver::Enable(void)
{
	servo.Enable();
	motor.Enable();
}

void RR_SolarRoverDriver::Turn(int _degr)
{
	//Take desired change in heading and use motor and servo to turn
}

void RR_SolarRoverDriver::Speed(int _spd)
{
	int _motor_spd, _motor_dir;
	// Do desired calcs for _motor_spd


	//Change motor speed
	motor.Speed(_motor_spd, _motor_dir);
}

void RR_SolarRoverDriver::Align(void)
{
	//Write Code here to use servo to adjust wheel radius in order to keep moving straight
}


RR_SolarRoverDriver::~RR_SolarRoverDriver(void)
{
}