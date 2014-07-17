#pragma once
#include "RR_CommonDefines.h"
#include <Arduino.h>

class RR_Motor
{
  public:  
    // CONSTRUCTORS
	RR_Motor(); // Default pin selection.
	RR_Motor(unsigned char M1DIR, unsigned char M1PWM, unsigned char M1FB,
                           unsigned char M2DIR, unsigned char M2PWM, unsigned char M2FB,
                           unsigned char nD2, unsigned char nSF); // User-defined pin selection. 
	~RR_Motor(void);
    
    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHZ. 
    void setM1Speed(int speed); // Set speed for M1.
    void setM2Speed(int speed); // Set speed for M2.
    void setSpeeds(int m1Speed, int m2Speed); // Set speed for both M1 and M2.
    unsigned int getM1CurrentMilliamps(); // Get current reading for M1. 
    unsigned int getM2CurrentMilliamps(); // Get current reading for M2.
    unsigned char getFault(); // Get fault reading.
    
  private:
    unsigned char _nD, _EN;
    unsigned char _M1DIR;
    unsigned char _M2DIR;
	static const unsigned char _M1PWM = MOTOR1_PWM_PIN;
	static const unsigned char _M2PWM = MOTOR2_PWM_PIN;
    unsigned char _nSF;
    unsigned char _M1FB;
    unsigned char _M2FB;
};


