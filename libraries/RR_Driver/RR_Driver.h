#pragma once
#include <RR_Motor\RR_Motor.h>
#include <RR_Receiver\RR_Receiver.h>
class RR_Driver
{
public:
	RR_Driver(void);
	void driveManual(void);
	//void driveAuto(message passed vector with gps location, heading, bearing);
	RR_Receiver receiver;
	RR_Motor motors;
	void Enable(void);
	~RR_Driver(void);
private:
	int leftSpeed, rightSpeed;
};

