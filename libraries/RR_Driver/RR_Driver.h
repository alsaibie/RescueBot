#pragma once
#include <RR_Motor\RR_Motor.h>
#include <RR_Receiver\RR_Receiver.h>
class RR_Driver
{
public:
	RR_Driver(void);
	RR_Driver(RR_Receiver *rec);
	//RR_Driver(Pointers to IMU, Encoder, GPS, Motor)
	//RR_Driver(Pointer to Receiver object) or combine
	void driveManual(void);
	void driveAuto(void);
	RR_Receiver *_receiver;
	~RR_Driver(void);
};

