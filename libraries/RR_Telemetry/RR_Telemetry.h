#pragma once
#include "RR_CommonDefines.h"
#include <RR_CommonData\RR_CommonData.h>
#include <Arduino.h>


class RR_Telemetry
{
  public:  
    // CONSTRUCTORS
	 RR_Telemetry(RR_AltimeterData_t *data);
	~RR_Telemetry(void);

  private:
	  RR_AltimeterData_t *telemetryData;

};



