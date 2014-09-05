#pragma once
#include "../RR_Motor/RR_Motor.h"

#if USE_RECEIVER
#include "../RR_Receiver/RR_Receiver.h"
#else
	#include "../Encoder/Encoder.h"
	#include "../RR_Encoder/RR_Encoder.h"
#endif

#include "../RR_CommonDefines/RR_CommonDefines.h"
#include "../RR_CommonData/RR_CommonData.h"
#define ACCELERATION_LIMIT	2000 //Points per second
#define MAXSPEEDCHANGE		100
typedef struct joystick_t : RR_TelemetryIncomingMessage_t::Joystick_t{

};

class RR_Driver
{
public:
	RR_Driver(RR_NavigationData_t *data);
	void driveManual(void);
	void driveManual(joystick_t data, uint16_t SamplingRate);
	void driveManual(uint16_t SamplingRate);
	void setFree(void);
	void driveAutonomous(RR_GPSData_t &gpsdata, RR_IMUData_t &imudata, RR_LoggerData_t &loggerdata, uint16_t SamplingRateE);
	void Enable(void);
	void Stop(void);
	NavigatingState_t NavigatingState;

	~RR_Driver(void);


private:

	//Driving Routines
	void cruiseModeSimple(int8_t bearing);
	void cruiseModeAdvanced(int8_t bearing);
	void tipoverMode(RR_IMUData_t &imudata); //When on the side and need to create imbalance 
	void wigglingMode(Speed_t speedlevel = MEDIUM, uint8_t repeats = 1); //When Stuck and both wheels arent's moving much or both are free wheeling.
	void reciprocatingMode(Speed_t speedleve = MEDIUM); //When not moving and one wheel is freewheeling.
	int16_t accLimit(int16_t speed, int16_t speedOld, uint16_t SamplingRate);
	//Situational Awareness Checks
	//Situation Code TIPPED or WHEELS (LEFT_RIGHT:STALL, FREE or RUN(IN MOTION)) 
	typedef enum {CLEAR=0, TIPPED, 
		STALL_STALL, STALL_FREE, STALL_RUN, 
		FREE_STALL, FREE_FREE, FREE_RUN, 
		RUN_STALL, RUN_FREE} Situation_t;
	void isObstacled(RR_IMUData_t &imudata, Situation_t &situation);
	//Navigation Tasks
	int8_t getdHeading(RR_IMUData_t &imudata, RR_GPSData_t &gpsdata);		
	
	//Internal Components
#if USE_RECEIVER
	RR_Receiver receiver;
#else
	RR_Encoder speedometer;
	int16_t leftActualSpeed, rightActualSpeed;
#endif

	RR_Motor motors;
	int16_t leftSpeed, leftSpeedOld, rightSpeedOld, rightSpeed;
	uint16_t EffectiveSamplingRate; //Effective Sampling Rate
	RR_NavigationData_t *navigationdata;
};

