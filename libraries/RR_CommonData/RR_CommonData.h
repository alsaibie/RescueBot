//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
Common Data Structures for Passing Information Between Software Components and Threads.
*/

#pragma once
#include <RR_CommonDefines/RR_CommonDefines.h>


//Altimeter Data
struct RR_AltimeterData_t
{
	float altitude;
	float temperature;
};

//Telemetry Data
struct RR_TelemetryData_t
{
};

//Navigation Data
struct RR_NavigationData_t
{
};

//GPS DATA
struct RR_GPSData_t
{
	float	Longitude;	char Lon;
	float	Latitude;	char Lat;
	float	LatitudeRadian, LongitudeRadian;
	float	LatitudeRadianTarget, LongitudeRadianTarget;
	float	Bearing;
	int		Speed;
	
	struct Date_t
	{
		uint8_t Year, Month, Day;
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
