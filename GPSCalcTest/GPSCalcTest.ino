
#include <RR_CommonData.h>
RR_GPSData_t *gpsData;
#define TARGET_LAT 3346.64750f  //N  ddmm.mmmm
#define TARGET_LON - 8424.01783f //W dddmm.mmmm 
#define CUR_LAT 3347.342f  //N  ddmm.mmmm
#define CUR_LON  - 8424.003f //W dddmm.mmmm 
void setup()
{
	delay(2000);
	gpsData = new RR_GPSData_t;
	gpsData->targetLatidude=TARGET_LAT;
	gpsData->targetLongitude=TARGET_LON;
	gpsData->LatitudeRadianTarget=toRadians(TARGET_LAT);
	gpsData->LongitudeRadianTarget=toRadians(TARGET_LON);
	gpsData->LatitudeRadian=toRadians(CUR_LAT);
	gpsData->LongitudeRadian=toRadians(CUR_LON);
	getBearing();

}

void loop()
{
	
  /* add main program code here */

}
void getBearing(void)
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

	// Using the formula for bearing to calculate the required current heading.
	float y = sin(dlong)*cos(latTarget);
	float x = cos(lat)*sin(latTarget)-sin(lat)*cos(latTarget)*cos(dlong);
	
	gpsData->Bearing = fmod((atan2(y,x)*57.295779513+360),360); // Multiplied by constant to convert to decimal degrees.
	if(DBUG){
		Serial.print("Bearing: "); Serial.println(gpsData->Bearing);
		Serial.print("d2Target: "); Serial.println(gpsData->DistanceToTarget);
	}
}


float toRadians(float coordinate)
{
  float min = fmod(coordinate, 100.0)/60.0;
  float deg = float(int(coordinate/100));
  return (deg+min)*PI/180.0;
}