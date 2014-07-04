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

	uint8_t LaunchState;
	uint8_t NavigationalState;
	float Latitude;
	float Longitude;
	uint16_t DistanceToTarget;
	int8_t Heading;
	int16_t	Bearing;
	uint16_t Altitude;	
}RR_TelemetryOutgoingMessage_t;

typedef struct RR_TelemetryIncomingMessage_t
{
    struct Joystick_t{

        struct Pad_t{
			int8_t Vertical, Horizontal;
        }Pad_A, Pad_B;
    }Joystick;
	
}RR_TelemetryIncomingMessage_t;

//Logger Data
struct RR_LoggerData_t
{
	struct GPSData_t{
		float	Longitude;	char *Lon;
		float	Latitude;	char *Lat;
		bool		fix;
		int16_t		Bearing;
		uint16_t	DistanceToTarget;
	}GPS;
	struct Time_t
	{
		uint8_t Hour, Minute, Seconds;
		//TODO:Change to HHMMSS Format but would need parsing
	}GPSTime;
	uint16_t Altitude;

	struct State_t{
		MainState_t mainstate;
		NavigatingState_t navstate;
	}State;

	struct Navigation_t{
		int16_t speed;
	}Navigation;
};

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
	float	Longitude;	char *Lon;
	float	Latitude;	char *Lat;
	float	LatitudeRadian, LongitudeRadian;
	float	LatitudeRadianTarget, LongitudeRadianTarget;
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

