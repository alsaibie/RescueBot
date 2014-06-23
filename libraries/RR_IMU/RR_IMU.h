/***************************************************************************
  This is a library for the GaTech 2014 RescueRobot Common board

  Based on the Adafruit 10DOF Breakout library. 
  However, this library only deals with the 9DOF IMU (mag,acc,gyro).
  The pressure sensor
	
  Written by Ali AlSaibie
 ***************************************************************************/
#pragma once
#include <RR_CommonDefines\RR_CommonDefines.h>
#include <RR_CommonData\RR_CommonData.h>
#include <Adafruit_Sensor\Adafruit_Sensor.h>
#include <Adafruit_LSM303DLHC\Adafruit_LSM303_U.h>
#include <Adafruit_10DOF\Adafruit_10DOF.h>
#include <Adafruit_L3GD20_U\Adafruit_L3GD20_U.h>
#include <Arduino.h>


class RR_IMU
{
  public:  
    //Constructors
	RR_IMU(void);
	RR_IMU(RR_IMUData_t *data);
	//Sensor Objects
	Adafruit_10DOF dof;
	Adafruit_LSM303_Accel_Unified accel;
	Adafruit_LSM303_Mag_Unified mag;
	Adafruit_L3GD20_Unified gyro;
	
	//RescueBot Methods
	void initIMU(void);
	void updateIMU(void);
	void DisplayIMUDetails(void);

	//Handlers
	sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_event_t gyro_event;
    sensors_vec_t   orientation;

	//Destructor
	~RR_IMU(void);
  private:
	  RR_IMUData_t *imuData;

};


