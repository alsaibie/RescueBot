#include "RR_Servo.h"

RR_Servo::RR_Servo(void)
{

}

void RR_Servo::Enable(void)
{
	attach(SERVO_CONTROL_PIN);
}

void RR_Servo::setAngle(int _angle)
{
	write(_angle);
}

int RR_Servo::getAngle(void)
{
	return read();
}

void RR_Servo::Release(void)
{
	detach();
}

RR_Servo::~RR_Servo(void)
{
}
