#include "RR_Altimeter.h"

RR_Altimeter::RR_Altimeter(void): bmp(18001)
{

}

void RR_Altimeter::initAltimeter(void)
{
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }
}

void RR_Altimeter::updateAltimeter(void)
{
	bmp.getEvent(&altimeter_event);
	if(DBUG)
	{
		if (altimeter_event.pressure)
		  {
			/* Display atmospheric pressure in hPa */
			Serial.print(F("PRESS "));
			Serial.print(altimeter_event.pressure);
			Serial.print(F(" hPa, "));
			/* Display ambient temperature in C */
			float temperature;
			bmp.getTemperature(&temperature);
			Serial.print(temperature);
			Serial.print(F(" C, "));
			/* Then convert the atmospheric pressure, SLP and temp to altitude    */
			/* Update this next line with the current SLP for better results      */
			float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
			Serial.print(bmp.pressureToAltitude(seaLevelPressure,
												altimeter_event.pressure,
												temperature)); 
			Serial.println(F(" m"));
		  }
	}
}

void RR_Altimeter::DisplayAltimeterDetails(void)
{
  sensor_t sensor;

  bmp.getSensor(&sensor);
  Serial.println(F("-------- PRESSURE/ALTITUDE ---------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" hPa"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" hPa"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" hPa"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

RR_Altimeter::~RR_Altimeter(void)
{
}
