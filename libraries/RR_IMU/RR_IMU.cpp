// Constructors ////////////////////////////////////////////////////////////////
#include "RR_IMU.h"
RR_IMU::RR_IMU(RR_IMUData_t *data):accel(30301), mag(30302), gyro(20), dof()
{
	imuData=data;
}

void RR_IMU::initIMU(void)
{
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
}
void RR_IMU::updateIMU(void)
{
	accel.getEvent(&accel_event);
	imuData->accelerometer.x=accel_event.acceleration.x;
	imuData->accelerometer.y=accel_event.acceleration.y;
	imuData->accelerometer.z=accel_event.acceleration.z;

    mag.getEvent(&mag_event);
	imuData->magnetometer.x=mag_event.magnetic.x;
	imuData->magnetometer.y=mag_event.magnetic.y;
	imuData->magnetometer.z=mag_event.magnetic.z;

	gyro.getEvent(&gyro_event);
	imuData->gyro.x=gyro_event.gyro.x;
	imuData->gyro.y=gyro_event.gyro.y;
	imuData->gyro.z=gyro_event.gyro.z;

	dof.fusionGetOrientation(&accel_event, &mag_event, &orientation);
	imuData->fused.roll=orientation.roll;
	imuData->fused.pitch=orientation.pitch;
	imuData->fused.heading=orientation.heading;

	if(DBUG)
	{
		/* 'orientation' should have valid .roll and .pitch fields */
		Serial.print(F("Orientation: "));
		Serial.print(orientation.roll);
		Serial.print(F(" "));
		Serial.print(orientation.pitch);
		Serial.print(F(" "));
		Serial.print(orientation.heading);
		Serial.println(F(""));
	}

}
void RR_IMU::DisplayIMUDetails(void)
{
  sensor_t sensor;
  
  accel.getSensor(&sensor);
  Serial.println(F("----------- ACCELEROMETER ----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" m/s^2"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  gyro.getSensor(&sensor);
  Serial.println(F("------------- GYROSCOPE -----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" rad/s"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  
  mag.getSensor(&sensor);
  Serial.println(F("----------- MAGNETOMETER -----------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" uT"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" uT"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" uT"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

RR_IMU::~RR_IMU(void)
{
}
