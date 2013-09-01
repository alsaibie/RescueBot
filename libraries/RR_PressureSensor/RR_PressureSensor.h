#pragma once
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <../Adafruit_BMP085/Adafruit_BMP085.h>
#include <../RunningAverage/RunningAverage.h>
#include <../RR_CommonDefines/RR_CommonDefines.h>


class RR_PressureSensor: public Adafruit_BMP085
{
public:
	RunningAverage SensorAverage;				//Define Running Average Object
	
	RR_PressureSensor(void);//:SensorAverage(PRESSURERUNNINGAVGSIZE)
	void Enable(void); //To Turn 
	void Update(void);
	float getAltitude(void);
	void Disable(void);
	
	bool LiftOff_FLAG;
	bool Landing_FLAG;
	float BaseAltitude;
	float MaxAltitude;
	int BaseAltitudeWait; // counter
	float AverageAltitude;
};

