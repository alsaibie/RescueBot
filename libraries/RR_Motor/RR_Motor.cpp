#include "RR_Motor.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_Motor::RR_Motor(void)
{
  //Pin map
  _nD = MOTOR_D_PIN;
  _EN = MOTOR_ENABLE_PIN;
  _M1DIR = M1DIR_PIN;
  _M2DIR = M2DIR_PIN;
  _nSF = MOTOR_SF_PIN;
  _M1FB = MOTOR1_FB_PIN; 
  _M2FB = MOTOR2_FB_PIN;
}

RR_Motor::RR_Motor(unsigned char M1DIR, unsigned char M1PWM, unsigned char M1FB,
                                               unsigned char M2DIR, unsigned char M2PWM, unsigned char M2FB,
                                               unsigned char nD, unsigned char nSF)
{
  //Pin map
  //PWM1 and PWM2 cannot be remapped because the library assumes PWM is on timer1
  _nD = nD;
  _M1DIR = M1DIR;
  _M2DIR = M2DIR;
  _nSF = nSF;
  _M1FB = M1FB; 
  _M2FB = M2FB;
}

// Public Methods //////////////////////////////////////////////////////////////
void RR_Motor::init()
{
// Define pinMode for the pins and set the frequency for timer1.
  pinMode(_M1DIR,OUTPUT);
  pinMode(_M1PWM,OUTPUT);
  pinMode(_M1FB,INPUT);
  pinMode(_M2DIR,OUTPUT);
  pinMode(_M2PWM,OUTPUT);
  pinMode(_M2FB,INPUT);
  pinMode(_nD,OUTPUT);
  pinMode(_EN,OUTPUT);
  digitalWrite(_nD,HIGH); // default to on
  digitalWrite(_EN,HIGH); // default to on
  pinMode(_nSF,INPUT);
  analogWriteFrequency(_M1PWM,20500);
  analogWriteFrequency(_M2PWM,20500);
}
// Set speed for motor 1, speed is a number betwenn -400 and 400
void RR_Motor::setM1Speed(int speed)
{
  //speed=-speed; //Because the wires are flipped
  unsigned char reverse = 0;
  
  if (speed > MOTOR_MAX_SPEED)  // Max PWM dutycycle
    speed = MOTOR_MAX_SPEED;
	
  if (speed < -MOTOR_MAX_SPEED)  // Max PWM dutycycle
    speed = -MOTOR_MAX_SPEED;
  if (speed < 0)
  {
    speed = MOTOR_MAX_SPEED+speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }

    
  analogWrite(_M1PWM,speed * 255 / MOTOR_MAX_SPEED); // default to using analogWrite, mapping 350 to 255
  if (reverse)
    digitalWrite(_M1DIR,HIGH);
  else
    digitalWrite(_M1DIR,LOW);
}

// Set speed for motor 2, speed is a number betwenn -400 and 400
void RR_Motor::setM2Speed(int speed)
{
	uint16_t leftOffset =0;
	//speed=-speed; //Because the wires are flipped
  unsigned char reverse = 0;
    
  if (speed > MOTOR_MAX_SPEED)  // Max PWM dutycycle
    speed = MOTOR_MAX_SPEED;
	
  if (speed < -MOTOR_MAX_SPEED)  // Max PWM dutycycle
    speed = -MOTOR_MAX_SPEED;

  if (speed < 0)
  {
    speed = MOTOR_MAX_SPEED+speed;   // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }

  analogWrite(_M2PWM,(speed * 255 / MOTOR_MAX_SPEED)); // default to using analogWrite, mapping 400 to 255
  if (reverse)
    digitalWrite(_M2DIR,HIGH);
  else
    digitalWrite(_M2DIR,LOW);
}

// Set speed for motor 1 and 2
void RR_Motor::setSpeeds(int m1Speed, int m2Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
}

// Return motor 1 current value in milliamps.
unsigned int RR_Motor::getM1CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 525 mV per A = 9 mA per count
  return analogRead(MOTOR1_FB_PIN) * 9;
}

// Return motor 2 current value in milliamps.
unsigned int RR_Motor::getM2CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 525 mV per A = 9 mA per count
  return analogRead(MOTOR2_FB_PIN) * 9;
}

// Return error status
unsigned char RR_Motor::getFault()
{
  return !digitalRead(_nSF);
}


RR_Motor::~RR_Motor(void)
{
}
