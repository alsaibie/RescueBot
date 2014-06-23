#pragma once
#include <RR_Motor\RR_Motor.h>
#include <RR_Encoder\RR_Encoder.h>
#include <RR_Receiver\RR_Receiver.h>
#include <RR_CommonData\RR_CommonData.h>
#include <RR_CommonDefines\RR_CommonDefines.h>

class RR_Driver
{
public:
	RR_Driver(void);
	void driveManual(void);
	void driveAutonomous(RR_NavigationData_t &navigationdata, RR_GPSData_t &gpsdata, RR_IMUData_t &imudata, RR_LoggerData_t &loggerdata);
	void Enable(void);
	NavigatingState_t NavigatingState;
	~RR_Driver(void);


private:

	//Driving Routines
	void cruiseModeSimple(uint8_t bearing);
	void cruiseModeAdvanced(uint8_t bearing);
	void tipoverMode(RR_IMUData_t &imudata); //When on the side and need to create imbalance 
	void wigglingMode(Speed_t speedlevel = MEDIUM); //When Stuck and both wheels arent's moving much or both are free wheeling.
	void reciprocatingMode(Speed_t speedleve = MEDIUM); //When not moving and one wheel is freewheeling.
	//Situational Awareness Checks
	//Situation Code TIPPED or WHEELS (LEFT_RIGHT:STALL, FREE or RUN(IN MOTION)) 
	typedef enum {CLEAR=0, TIPPED, 
		STALL_STALL, STALL_FREE, STALL_RUN, 
		FREE_STALL, FREE_FREE, FREE_RUN, 
		RUN_STALL, RUN_FREE} Situation_t;
	Situation_t isObstacled(RR_IMUData_t &imudata);
	//Navigation Tasks
	int8_t getdHeading(RR_IMUData_t &imudata, RR_GPSData_t &gpsdata);		
	
	//Internal Components
	RR_Encoder speedometer;
	RR_Receiver receiver;
	RR_Motor motors;
	int8_t leftSpeed, rightSpeed;
};

