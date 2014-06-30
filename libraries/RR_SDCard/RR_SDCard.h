#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <RR_CommonDefines\RR_CommonDefines.h>
#include <RR_CommonData\RR_CommonData.h>
//define message types here
#define COORDINATE_MSG "$COORDINATE,"
#define SPEED_MSG "$SPEED,"
#define COURSE_MSG "$COURSE,"
#define ALTITUDE_MSG "$ALTITUDE,"
#define GPS_FIX_MSG "$GPS_FIX_TIME,"

class RR_SDCard
{
public:
	RR_SDCard(HardwareSerial *ser);
	void Initialize(void);
	void WriteMessage(float _msg_value, const char *_msg_type);
	void Write(const char *_str);
	//const char readFile(void); // Future implementation
	
	~RR_SDCard(void);
private:
	//char fileName[12];
	HardwareSerial *SD_HwSerial;
	//SoftwareSerial SD_Serial;
	

};

