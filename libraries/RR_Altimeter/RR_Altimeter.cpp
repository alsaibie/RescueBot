#include "RR_Altimeter.h"

RR_Altimeter::RR_Altimeter(RR_AltimeterData_t *data): bmp(18001)
{
	altimeterData=data;
	altimeterData->baseAltitude=0;
	altimeterData->maxAltitude=0;
	checkPeakInd=0;
	checkLandingInd=0;
	checkLandingcounter=0;
	if(!bmp.begin())
	  {
		/* There was a problem detecting the BMP180 ... check your connections */
		Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
		while(1);
	  }
}

void RR_Altimeter::initAltimeter(void)
{


  //TODO: take some readings for some time and record base altitude.
  int i;
  uint8_t samplesBaseAlt=10;
  for (i=0; i<samplesBaseAlt; i++){
	  updateAltimeter();
	  delay(500);
	  altimeterData->baseAltitude+=altimeterData->altitude;
  }
  altimeterData->baseAltitude=altimeterData->baseAltitude/samplesBaseAlt;
  if(DBUG) {
	  Serial.print("Base Altitude: ");
	  Serial.println(altimeterData->baseAltitude);
  }
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
			if((altimeterData->altitude)>(altimeterData->baseAltitude+LAUNCH_THRESHOLD)){
				altimeterData->Launched=true;
				if(DBUG) {Serial.println("Launched");}
			}
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
				if(DBUG) {Serial.println("Descending");}
				checkLandingAltitude=altimeterData->altitude;
			}
			break;
		case checkLANDING:

			//Check if altitude doesn't change for 1 minute using the sampling time
			//Option 1: Check 3 times every 20 seconds, the altitude didn't change more than LANDING_CHECK_THRESHOLD meters
			checkLandingcounter++;
						if(0){Serial.print("Landing Counter: ");
					Serial.println(checkLandingcounter);
					}
			if(SamplingTime*checkLandingcounter>2000L){
				TBUG
				checkLandingcounter=0;
				if (abs(altimeterData->altitude-checkLandingAltitude)<LANDING_CHECK_THRESHOLD){
					checkLandingInd++;
					if(DBUG){Serial.print("F: ");
					Serial.println(checkLandingInd);
					}
				}
				else{	
					checkLandingcounter=0;
				}

				checkLandingAltitude=altimeterData->altitude;

				if(checkLandingInd>3){
					altimeterData->Landed=true;
					if(DBUG) {Serial.println("Landed");}
					}
				}
			break;
	}
	if(DBUG)
	{
		if (altimeter_event.pressure)
		  {
			/* Display atmospheric pressure in hPa */
			//Serial.print(F("PRESS "));
			//Serial.print(altimeter_event.pressure);
			//Serial.print(F(" hPa, "));
			/* Display ambient temperature in C */
			//float temperature;
			//bmp.getTemperature(&temperature);
			//Serial.print(temperature);
			//Serial.print(F(" C, "));
			/* Then convert the atmospheric pressure, SLP and temp to altitude    */
			/* Update this next line with the current SLP for better results      */
			float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
			Serial.print("A:");
			Serial.print(bmp.pressureToAltitude(seaLevelPressure,
												altimeter_event.pressure,
												temperature)); 
			Serial.println(F("m"));
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
