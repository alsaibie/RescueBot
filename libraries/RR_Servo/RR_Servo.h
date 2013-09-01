#include <Servo.h>
#include <../RR_CommonDefines/RR_CommonDefines.h>

#pragma once
class RR_Servo :public Servo
{
public:
	RR_Servo(void);
	void Enable(void);
	void setAngle(int _angle);
	int getAngle(void);
	void Release(void);
	~RR_Servo(void);
};

