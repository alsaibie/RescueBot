#include <Filter.h>
#include <Encoder.h>
#include <RR_Encoder.h>
#include <RR_Motor.h>
#include <RR_Driver.h>
#include <RR_Receiver.h>
#include <RR_CommonDefines.h>
#include <RR_CommonData.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_LSM303_U.h>
#include <RR_IMU.h>

RR_NavigationData_t	navdata;
RR_Driver scorpio(&navdata);
RR_IMUData_t imudata;
RR_IMU imuS(&imudata);

void setup(void)
{
	scorpio.Enable();
  Serial.begin(115200);
  delay(2000);
  Serial.println(F("RR IMU Tester")); Serial.println("");
  
  /* Initialise the IMU */
  imuS.initIMU();
  	Serial.println("Test");
  delay(3000);
  /* Display some basic information on this IMU */
  imuS.DisplayIMUDetails();
  delay(1000);
}

void loop(void)
{

  imuS.updateIMU();
  //scorpio.driveManual();
  delay(10);
}
