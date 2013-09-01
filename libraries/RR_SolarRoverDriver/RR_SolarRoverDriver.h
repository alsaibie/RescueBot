//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
High Lever Motor and Servo commander for Solid Sphero Robot

Should be desgined to take desired heading change and speed and change motor and servo accordingly. 


Methods:
Enable(): Enable Motor and Servo
Turn(desired change in Course angle)

TODO:
Write it up
- Have Constructor take GPS object by reference to use in Align method. 
*/
#pragma once
#include <../RR_Motor/RR_Motor.h>
#include <../RR_Servo/RR_Servo.h>

class RR_SolarRoverDriver
{
public:
	RR_SolarRoverDriver(void);
	void Enable(void);
	void Turn(int _degr);
	void Speed(int _spd);
	void Align(void);
	~RR_SolarRoverDriver(void);

private:

	RR_Motor motor;
};
