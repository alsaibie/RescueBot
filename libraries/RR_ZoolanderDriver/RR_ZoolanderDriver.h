//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
High Lever Motor and Servo commander for Zoolander Robot

Should be desgined to take desired heading change and speed and change motor and servo accordingly. 

Also used to align robot autonomously. 

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

class RR_ZoolanderDriver
{
public:
	RR_ZoolanderDriver(void);
	void Enable(void);
	void Turn(int _degr);
	void Speed(int _spd);
	~RR_ZoolanderDriver(void);
	void Align();

private:

	RR_Motor motor;
	RR_Servo servo;
	
};

