//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
* Common Data Structures for Passing Information Between Software Components and Threads.
*/

#pragma once
#include <RR_CommonDefines/RR_CommonDefines.h>

//Telemetry Data
typedef struct RR_TelemetryOutgoingMessage_t
{
    struct Data_t
    { 
		uint16_t Year;
		uint8_t Month, Day;
    }Date;  

    struct Time_t
    {
		uint8_t Hour, Minute, Seconds;
    }Time;

	uint8_t launchState;
	uint8_t navigationalState;
	uint32_t latitude;
	uint32_t longitude;
	uint16_t distanceToTarget;
	int8_t heading;
	uint16_t altitude;
	
}RR_TelemetryOutgoingMessage_t;

typedef struct RR_TelemetryIncomingMessage_t
{
	uint8_t msg_timestamp; //time
	
}RR_TelemetryIncomingMessage_t;

//Altimeter Data
struct RR_AltimeterData_t
{
	uint32_t baseAltitude, maxAltitude;
	uint32_t altitude;
	int16_t temperature;
	bool Launched, Peaked, Landed;
};

//Navigation Data
struct RR_NavigationData_t
{
};

//GPS DATA
struct RR_GPSData_t
{
	
	uint32_t	Longitude;	char Lon;
	uint32_t	Latitude;	char Lat;
	uint32_t	LatitudeRadian, LongitudeRadian;
	uint32_t	LatitudeRadianTarget, LongitudeRadianTarget;
	bool		fix;
	int16_t			Bearing;
	uint16_t		DistanceToTarget;
	uint8_t			Speed;
	
	struct Date_t
	{
		uint16_t Year;
		uint8_t Month, Day;
		//TODO:Change to YYMMDD Format but would need parsing
	}Date;
	
	struct Time_t
	{
		uint8_t Hour, Minute, Seconds;
		//TODO:Change to HHMMSS Format but would need parsing
	}Time;

};

//IMU Data
struct RR_IMUData_t
{
	struct gyrodata_t
	{
		float x,y,z;
	}gyro;

	struct acceldata_t
	{
		float x,y,z;
	}accelerometer;
	
	struct magdata_t
	{
		float x,y,z;
	}magnetometer;

	struct fuseddata_t
	{
		float pitch, roll, heading;
	}fused;

};

//Logger Data
struct RR_LoggerData_t
{

};
