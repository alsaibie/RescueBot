#include "RR_GPS.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_GPS::RR_GPS(RR_GPSData_t *data):Adafruit_GPS(&GPS_Serial)
{
	gpsData=data;
	gpsData->LatitudeRadianTarget=toRadians(TARGET_LAT);
	gpsData->LongitudeRadianTarget=toRadians(TARGET_LON);

	//Default to low on pin at constructor
	pinMode(GPS_ENABLE_PIN, OUTPUT);
	digitalWrite(GPS_ENABLE_PIN,LOW);
}
void RR_GPS::Disable(void)
{
  digitalWrite(GPS_ENABLE_PIN,LOW);
}
void RR_GPS::Enable(void)
{
  digitalWrite(GPS_ENABLE_PIN,HIGH);

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  begin(9600);
  
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  
  // Set the update rate
  sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  sendCommand(PGCMD_ANTENNA);
}


void RR_GPS::Update(void)
{
	char c =read();

	if(0)
	{if (c) Serial.print(c);}

	// If a sentence is received, we can check the checksum, parse it...
    
	if (newNMEAreceived()) 
	{
		if(0)
		{
			Serial.println("NewNMEA");
			Serial.println(lastNMEA());   // this also sets the newNMEAreceived() flag to false
		}

		if (!parse(lastNMEA()))   // this also sets the newNMEAreceived() flag to false
		{
			newGPSData=false;
			return;
		}  // we can fail to parse a sentence in which case we should just wait for another
		else
		{
					newGPSData=true;
		}
	}
}

void RR_GPS::getData(void)
{
		getPosition();
		getBearing();
		getTimeDate();
}


void RR_GPS::getPosition(void)
{
	gpsData->Latitude=latitude;
	gpsData->Longitude=longitude;
	gpsData->Lat=lat;
	gpsData->Lon=lon;
	//Convert to Radians and store accordingly.
	gpsData->LatitudeRadian=toRadians(latitude);
	gpsData->LongitudeRadian=toRadians(longitude);

}

void RR_GPS::getTimeDate(void)
{
	gpsData->Date.Year=year;
	gpsData->Date.Month=month;
	gpsData->Date.Day=day;

	gpsData->Time.Hour=hour;
	gpsData->Time.Minute=minute;
	gpsData->Time.Seconds=seconds;

}
void RR_GPS::getBearing(void)
{

	// Using Haversine formula for calculating Over-the-surface distance
	// Source: http://www.movable-type.co.uk/scripts/latlong.html
	int R = 6371;   // Mean Earth radius

	float lat = gpsData->LatitudeRadian;
	float latTarget = gpsData->LatitudeRadianTarget;
	float lon = gpsData->LongitudeRadian;
	float lonTarget = gpsData->LongitudeRadianTarget;

	float dlat = latTarget-lat;
	float dlong = lonTarget-lon;

	float a = sin(dlat/2)*sin(dlat/2)+sin(dlong/2)*sin(dlong/2)*cos(lat)*cos(latTarget);
	float c = 2*atan2(sqrt(a),sqrt(1-a));
	
	gpsData->DistanceToTarget = R*c;

	// Using the formula for bearing to calculate the required current heading.
	float y = sin(dlong)*cos(latTarget);
	float x = cos(lat)*sin(latTarget)-sin(lat)*cos(latTarget)*cos(dlong);
	
	gpsData->Bearing = fmod((atan2(y,x)*57.295779513+360),360); // Multiplied by constant to convert to decimal degrees.
  
}


float RR_GPS::toRadians(float coordinate)
{
  float min = fmod(coordinate, 100.0)/60.0;
  float deg = float(int(coordinate/100));
  return (deg+min)*PI/180.0;
}

RR_GPS::~RR_GPS(void)
{
}
