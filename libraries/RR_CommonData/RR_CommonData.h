//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
* Common Data Structures for Passing Information Between Software Components and Threads.
*/

#pragma once
#include "../RR_CommonDefines/RR_CommonDefines.h"
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
	uint8_t  fix;
	float Latitude;
	float Longitude;
    float targetLatidude, targetLongitude;
	uint16_t DistanceToTarget;
	int16_t Heading;
	int16_t	Bearing;
	uint16_t Altitude, baseAltitude, maxAltitude;
	uint32_t ellapsedm;

}RR_TelemetryOutgoingMessage_t;

typedef struct RR_TelemetryIncomingMessage_t
{
    struct Joystick_t{
        struct Pad_t{
			int16_t X_Axis, Y_Axis;
        }Pad_Left, Pad_Right;
    }Joystick;

	uint8_t bottum1;
	uint8_t StartState;
    uint8_t DriveMode;
}RR_TelemetryIncomingMessage_t;

//Logger Data
struct RR_LoggerData_t
{
	struct GPSData_t{
		float	Longitude;	char Lon;
		float	Latitude;	char Lat;
		bool		fix;
		int16_t		Bearing;
		uint16_t	DistanceToTarget, DistanceTravelled;
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
		int16_t heading;
	}Navigation;
};

//Altimeter Data
struct RR_AltimeterData_t
{
	uint32_t baseAltitude;
	uint32_t  maxAltitude, altitude;
	int16_t temperature;
	bool Launched, Peaked, Landed;
};

struct WhatToLog_t
{
	bool altitude, gps, navigation, imu;
};
//Navigation Data
struct RR_NavigationData_t
{
	DriveMode_t driveMode;
	int16_t leftMotorSpeed, rightMotorSpeed;
};

//GPS DATA
struct RR_GPSData_t
{
	float	Longitude;	char Lon;
	float	Latitude;	char Lat;
    float targetLatidude, targetLongitude;
	float	LatitudeRadian, LongitudeRadian;
	float	LatitudeRadianTarget, LongitudeRadianTarget;
	bool		fix;
	int16_t			Bearing;
	uint16_t		DistanceToTarget, DistanceTravelled;
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

	float headingFiltered;
};

