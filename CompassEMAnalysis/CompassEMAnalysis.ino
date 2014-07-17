#include <RunningAverage.h>
#include <RR_SDCard.h>
#include <RR_Receiver.h>
#include <Encoder.h>
#include <RR_Motor.h>
#include <Wire.h>
#include <RR_Driver.h>
#include <RR_CommonDefines.h>
#include <RR_CommonData.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <RR_Encoder.h>
#include <RR_IMU.h>

RR_IMUData_t imuData;
RR_NavigationData_t navigationData;
RR_LoggerData_t loggerData;

RR_Driver driver(&navigationData);
RR_IMU imu(&imuData);
RR_SDCard sdcard(&loggerData);

void setup()
{
	driver.Enable();
	imu.initIMU();
	delay(1000);
	sdcard.Initialize();		
	delay(2000);
  /* add setup code here */

}

void loop()
{
	driver.driveManual();
	imu.updateIMU();
	//Write to SD card directly, bypass sdcard class for now
	Serial2.print("EMT,");
	Serial2.print(imuData.fused.heading);
	comma;
	Serial2.print(navigationData.leftMotorSpeed);
	comma;
	Serial2.print(navigationData.rightMotorSpeed);
	hashEnd;
	delay(50);
  /* add main program code here */
}
