#pragma once
#include <RR_CommonDefines\RR_CommonDefines.h>
#include <Adafruit_Sensor\Adafruit_Sensor.h>
#include <Adafruit_BMP085_Unified\Adafruit_BMP085_U.h>
#include <Arduino.h>

class RR_Altimeter
{
	public:  
    //Constructors
	RR_Altimeter(void);

	//Sensor Objects
	Adafruit_BMP085_Unified       bmp;
	
	//RescueBot Methods
	void initAltimeter(void);
	void updateAltimeter(void);
	void DisplayAltimeterDetails(void);
	//TODO: add sensoraverage object
	//Handlers
    sensors_event_t altimeter_event;

	//Destructor
	~RR_Altimeter(void);
  private:

};


