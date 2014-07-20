#include <RR_CommonDefines.h>
#include <RR_CommonData.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <Adafruit_LSM303_U.h>
#include <RR_IMU.h>


RR_IMUData_t imudata;
RR_IMU imu(&imudata);

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("RR IMU Tester")); Serial.println("");
  
  /* Initialise the IMU */
  imu.initIMU();
  delay(3000);
  /* Display some basic information on this IMU */
  imu.DisplayIMUDetails();
  delay(5000);
}

void loop(void)
{
  imu.updateIMU();
  delay(200);
}
