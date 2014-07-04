#pragma once
#include <RR_CommonDefines\RR_CommonDefines.h>
#include <RR_CommonData\RR_CommonData.h>

//define message types here
/* GPS Data Format 
* $GPS,mcutime,Fix,GPSTIME,Latitude,Lat,Longitude,Lon,DistanceToTarget,Bearing,#
*/
#define GPS_LOG_MSG "$GPS"

/* STATE Format 
* $STA,mcutime,MainState,NavigationalState,# 
*/
#define STATE_LOG_MSG "$STA"

/* Navigational Data Format 
* $NAV,mcutime,Speed,#
*/
#define NAVIGATION_LOG_MSG "$NAV"

/* Altitude Format 
* $ALT,mcutime,Altitude,#
*/
#define ALTITUDE_LOG_MSG "$ALT"
#define comma Serial2.print(",")
#define hashEnd Serial2.println("#")
// # Signifies end of message
class RR_SDCard
{
	public:
	RR_SDCard(RR_LoggerData_t *data);
	void Initialize(void);
	void WriteMessage(float _msg_value, const char *_msg_type);
	void Write(const char *_str);
	void updateLog(void);
	//const char readFile(void); // Future implementation
	~RR_SDCard(void);

	private:
	RR_LoggerData_t *loggerData;
};

