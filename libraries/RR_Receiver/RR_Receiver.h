#pragma once
#include "RR_CommonDefines.h"
#include <Arduino.h>

#define _dTHR THRO_RX_PIN
#define _dAIL AILE_RX_PIN
#define _dELE ELEV_RX_PIN
#define _dRUD RUDD_RX_PIN

enum CHANNEL {THRO, AILE, ELEV, RUDD};

class RR_Receiver
{
  public: 
	  RR_Receiver();
	  //make vector to store data
	  int getSignal(CHANNEL _chan);
//TODO: Calibration Routine, Vector to Save all signals so function is called once only with pointer
};