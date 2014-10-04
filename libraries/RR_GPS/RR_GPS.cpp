#include "RR_GPS.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_GPS::RR_GPS(RR_GPSData_t *data):Adafruit_GPS(&GPS_Serial)
{
	gpsData=data;
	gpsData->targetLatidude=TARGET_LAT;
	gpsData->targetLongitude=TARGET_LON;
	gpsData->LatitudeRadianTarget=toRadians(TARGET_LAT);
	gpsData->LongitudeRadianTarget=toRadians(TARGET_LON);
	gpsData->LatitudeRadian=0; gpsData->LongitudeRadian=0;
	//Default to low on pin at constructor
	pinMode(GPS_ENABLE_PIN, OUTPUT);
	if(!DBUG) {digitalWrite(GPS_ENABLE_PIN,LOW);}
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
					gpsData->fix=fix;
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
	if(lat=='S') gpsData->Latitude*=-1;
	gpsData->Longitude=longitude;
	if(lon=='W') gpsData->Longitude*=-1;
	gpsData->Lat=lat;
	gpsData->Lon=lon;
	//Compute Distance Travelled
	float latRad = toRadians(gpsData->Latitude);
	float lonRad = toRadians(gpsData->Longitude);
	if(gpsData->LatitudeRadian != 0 && gpsData->LongitudeRadian !=0){
		gpsData->DistanceTravelled+=getDistance(gpsData->LatitudeRadian,latRad,gpsData->LongitudeRadian,lonRad);
	}

	//Convert to Radians and store accordingly.
	gpsData->LatitudeRadian		=	latRad;
	gpsData->LongitudeRadian	=	lonRad;

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
	int R = 6371000;   // Mean Earth radius

	float lat = gpsData->LatitudeRadian;
	float latTarget = gpsData->LatitudeRadianTarget;
	float lon = gpsData->LongitudeRadian;
	float lonTarget = gpsData->LongitudeRadianTarget;
	if(0){
		Serial.print("Rob Location: "); Serial.print(lat,6); Serial.print(","); Serial.println(lon,6);
		Serial.print("Target Location: "); Serial.print(latTarget,6); Serial.print(","); Serial.println(lonTarget,6);
	}
	float dlat = latTarget-lat;
	float dlong = lonTarget-lon;

	float a = sin(dlat/2)*sin(dlat/2)+sin(dlong/2)*sin(dlong/2)*cos(lat)*cos(latTarget);
	float c = 2*atan2(sqrt(a),sqrt(1-a));
	
	gpsData->DistanceToTarget = R*c;
	if(DBUG){Serial.println(gpsData->DistanceToTarget);}
	// Using the formula for bearing to calculate the required current heading.
	float y = sin(dlong)*cos(latTarget);
	float x = cos(lat)*sin(latTarget)-sin(lat)*cos(latTarget)*cos(dlong);
	
	gpsData->Bearing = fmod((atan2(y,x)*57.295779513+360),360); // Multiplied by constant to convert to decimal degrees.
  	if(0){
		Serial.print("Bearing: "); Serial.println(gpsData->Bearing);
		Serial.print("Distance to Target: "); Serial.println(gpsData->DistanceToTarget);
	}
}


float RR_GPS::toRadians(float coordinate)
{
  float min = fmod(coordinate, 100.0)/60.0;
  float deg = float(int(coordinate/100));
  return (deg+min)*PI/180.0;
  if(0){
	  Serial.println(min);
	  Serial.println(deg);
  }
}

RR_GPS::~RR_GPS(void)
{
}

uint16_t RR_GPS::getDistance(float lat0rad, float lon0rad, float lat1rad, float lon1rad){
	
	float dlat = lat1rad-lat0rad;
	float dlong = lon1rad-lon0rad;

	float a = sin(dlat/2)*sin(dlat/2)+sin(dlong/2)*sin(dlong/2)*cos(lat0rad)*cos(lat1rad);
	float c = 2*atan2(sqrt(a),sqrt(1-a));
	return (uint16_t)(a * c);
}