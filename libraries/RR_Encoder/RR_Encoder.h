#pragma once
#include "RR_CommonDefines.h"
#include <Arduino.h>
#include <Encoder\Encoder.h>
class RR_Encoder
{
  public:  
    // CONSTRUCTORS
	RR_Encoder(void);
	~RR_Encoder(void);

	void Update(void);
	void getSpeed(int *mot1, int *mot2); 
  private:
	//Encoder ENCA;
	Encoder ENCB;
	long oldPos1, newPos1, oldPos2, newPos2;

};


