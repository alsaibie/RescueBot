#pragma once
#include <RR_CommonDefines\RR_CommonDefines.h>
#include <RR_CommonData\RR_CommonData.h>
#include <Adafruit_GPS\Adafruit_GPS.h>
#include <Arduino.h>

class RR_GPS: public Adafruit_GPS
{
  public:  
    // CONSTRUCTORS
	RR_GPS();
	RR_GPS(RR_GPSData_t *data);
	void Enable(void);
	void Update(void);
	void getHeading(void);
	void getPosition(void);
	void getBearing(void);
	void getTimeDate(void);
	float toRadians(float coordinate); //Change the name to capture the "from" format
	~RR_GPS(void);
	bool newGPSData;
  private:
	  RR_GPSData_t *gpsData;

};





