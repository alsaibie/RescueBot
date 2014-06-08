#pragma once
#include "RR_CommonDefines.h"
#include <Arduino.h>

class RR_MOSFET
{
  public:  
	enum SWITCHMODE {ON, OFF};
	RR_MOSFET();
	void switchMOSFET(SWITCHMODE _mode);
	~RR_MOSFET(void);

  private:
};


