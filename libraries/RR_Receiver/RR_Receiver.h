#pragma once
#include "../RR_CommonDefines/RR_CommonDefines.h"
#include <Arduino.h>

typedef enum {THRO = 0, AILE = 1, ELEV = 2, RUDD = 3} CHANNEL;

class RR_Receiver
{
  public: 
	  RR_Receiver();
	  //RR_Receiver(6 Chan)
	  //RR_Receiver(3 Chan)
	  //make vector to store data
	  int getSignal(CHANNEL _chan);
	  //TODO: Remove outlier readings
//TODO: Calibration Routine, Vector to Save all signals so function is called once only with pointer
};