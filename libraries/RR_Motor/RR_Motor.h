//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
Low Level Motor Control with a simple H-Bridge class. Designed to Handle one motor with predefined hardware pin connections

Should be wrapped with a driver class specific to the robot being used.
Methods:

Enable(): Run in setup to put motor in forward ready mode
Speed(0:Stop-255:Max, FORWARD/BACKWARD)
Stop(): Quick stop for motor. 

TODO:
Improve speed change smoothness. Things to consider: change PWM frequency and/or have delayed inceremental analogwrite

*/
#pragma once
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <../RR_CommonDefines/RR_CommonDefines.h>

class RR_Motor
{
public:
	RR_Motor(void);
	void Enable(void);
	void Speed(int _spd, int _dir);
	void Stop(void);
	~RR_Motor(void);

private:
	int _current_dir, _current_spd;
};

