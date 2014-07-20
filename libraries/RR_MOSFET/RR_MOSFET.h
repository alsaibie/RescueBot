#pragma once
#include "../RR_CommonDefines/RR_CommonDefines.h"
#include <Arduino.h>

class RR_MOSFET
{
  public:  
	RR_MOSFET();
	void switchMOSFET(SwitchMode_t _mode);
	~RR_MOSFET(void);

  private:
};


