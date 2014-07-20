#pragma once
#include "../RR_CommonDefines/RR_CommonDefines.h"
#include "../RR_CommonData/RR_CommonData.h"
#include "../Adafruit_Sensor/Adafruit_Sensor.h"
#include "../Adafruit_BMP085_Unified/Adafruit_BMP085_U.h"
#include <Arduino.h>


class RR_Altimeter
{
	public:  
    //Constructors

	RR_Altimeter(RR_AltimeterData_t *data);
	//Sensor Objects
	Adafruit_BMP085_Unified       bmp;
	
	//RescueBot Methods
	void initAltimeter(void);
	void updateAltimeter(AltimeterTask_t altimeterTask=STANDBY, uint16_t SamplingTime=100L);
	void DisplayAltimeterDetails(void);
	//TODO: add sensoraverage object
	//Handlers
    sensors_event_t altimeter_event;
	//Destructor
	~RR_Altimeter(void);
  private:
	 RR_AltimeterData_t *altimeterData; 
	 uint8_t checkPeakInd;
	 uint32_t checkLandingAltitude;
};




