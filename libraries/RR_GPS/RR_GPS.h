#pragma once
/*
* GPS Class.
* GPS Location format of the Module DDMM.MMMM (Dec Min)
* GPS Time format hhmmss.sss
*/
#include "../RR_CommonDefines/RR_CommonDefines.h"
#include "../RR_CommonData/RR_CommonData.h"
#include "../Adafruit_GPS/Adafruit_GPS.h"

#include <math.h>
#include <Arduino.h>

class RR_GPS: public Adafruit_GPS
{
  public:  
    // CONSTRUCTORS
	RR_GPS(RR_GPSData_t *data);
	void Enable(void);
	void Disable(void);
	void Update(void);
	void getData(void);
	float toRadians(float coordinate); //Change the name to capture the "from" format
	~RR_GPS(void);
	bool newGPSData;

private:
	void getPosition(void);
	void getBearing(void);
	void getTimeDate(void);
	uint16_t getDistance(float lat0rad, float lon0rad, float lat1rad, float lon1rad);
	RR_GPSData_t *gpsData;

};





