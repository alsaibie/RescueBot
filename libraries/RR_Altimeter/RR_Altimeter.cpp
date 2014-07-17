#include "RR_Altimeter.h"

RR_Altimeter::RR_Altimeter(RR_AltimeterData_t *data): bmp(18001)
{
	altimeterData=data;
	altimeterData->baseAltitude=0;
	altimeterData->maxAltitude=0;
	checkPeakInd=0;
}

void RR_Altimeter::initAltimeter(void)
{
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while(1);
  }

  //TODO: take some readings for some time and record base altitude.
  int i;
  for (i=0; i<10; i++){
	  updateAltimeter();
	  delay(500);
	  altimeterData->baseAltitude+=altimeterData->altitude;
  }
  altimeterData->baseAltitude*=1/i;
}

void RR_Altimeter::updateAltimeter(AltimeterTask_t altimeterTask, uint16_t SamplingTime)
{
	bmp.getEvent(&altimeter_event);
	float temperature;
	bmp.getTemperature(&temperature);
	altimeterData->temperature=temperature;
	altimeterData->altitude=bmp.pressureToAltitude(SENSORS_PRESSURE_SEALEVELHPA, 
	altimeter_event.pressure, temperature);

	switch (altimeterTask)
	{
		case STANDBY:
			// Do Nothing
			break;
		case checkLAUNCH:
			if((altimeterData->altitude)>(altimeterData->baseAltitude+LAUNCH_THRESHOLD)? altimeterData->Launched=true : NULL);
			break;
		case checkPEAK:
			// If read altitude is greater than recorder max altitude, it would be the max
			if((altimeterData->altitude)>(altimeterData->maxAltitude)){
				altimeterData->maxAltitude=altimeterData->altitude;
				checkPeakInd=0;
			}
			// If read altitude is smaller than X% of the max altitude incerment counter. 
			else if((altimeterData->altitude)<(99/100)*(altimeterData->maxAltitude)){
				checkPeakInd++;
			}

			if(checkPeakInd==10)
			{
				altimeterData->Peaked=true;
			}
			break;
		case checkLANDING:
			//Check if altitude doesn't change for 1 minute using the sampling time
			break;
	}
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
