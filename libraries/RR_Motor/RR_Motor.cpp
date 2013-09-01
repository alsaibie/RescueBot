#include "RR_Motor.h"

RR_Motor::RR_Motor(void)
{
	_current_dir=0;
	_current_spd=0;
}

void RR_Motor::Enable(void)
{
	pinMode(MOTOR1_ENABLE_PIN, OUTPUT); 
	//Set Speed to Zero
	analogWrite(MOTOR1_ENABLE_PIN, 0);
	//Set Direction to Forward
	digitalWrite(MOTOR1_LOGIC1_PIN,LOW);
	digitalWrite(MOTOR1_LOGIC2_PIN,HIGH);
	
}

void RR_Motor::Speed(int _spd, int _dir)
{
	
	//Check if direction is changing
	if(_dir!=_current_dir)
	{
		_current_dir=_dir;
		//Stop first
		analogWrite(MOTOR1_ENABLE_PIN, 0);
		delay(30);

		if(_dir==1)
		{
			digitalWrite(MOTOR1_LOGIC1_PIN,LOW);
			digitalWrite(MOTOR1_LOGIC2_PIN,HIGH);
		}
		else if (_dir==0)
		{
			digitalWrite(MOTOR1_LOGIC1_PIN,HIGH);
			digitalWrite(MOTOR1_LOGIC2_PIN,LOW);
		}
		
	}
	//Change Speed
	analogWrite(MOTOR1_ENABLE_PIN, _spd);
	_current_spd=_spd;
}


void RR_Motor::Stop(void)
{
	
	digitalWrite(MOTOR1_LOGIC1_PIN,LOW);
	digitalWrite(MOTOR1_LOGIC2_PIN,LOW);
	analogWrite(MOTOR1_ENABLE_PIN, 0);
}

RR_Motor::~RR_Motor(void)
{
}
