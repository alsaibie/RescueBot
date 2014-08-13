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

void RR_IMU::initAHRS(void)
{

}

void RR_IMU::updateIMU(void)
{
	int avgcount = 20;
	float tempavg = 0;

		accel.getEvent(&accel_event);
		accelFil_event.acceleration.x=imuFilterX.step(accel_event.acceleration.x);
		accelFil_event.acceleration.y=imuFilterY.step(accel_event.acceleration.y);
		accelFil_event.acceleration.z=imuFilterZ.step(accel_event.acceleration.z);
		mag.getEvent(&mag_event);
		gyro.getEvent(&gyro_event);
		//dof.fusionGetOrientation(&accelFil_event, &mag_event, &orientation);
		//dof.fusionGetOrientation(&accel_event, &mag_event, &orientation);
		//dof.accelGetOrientation(&accelFil_event,&orientation);
		//dof.magTiltCompensation(SENSOR_AXIS_Z, &mag_event, &accelFil_event);
		dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation);

	imuData->fused.heading=-orientation.heading;
	imuData->fused.roll=orientation.roll;
	imuData->fused.pitch=orientation.pitch;


	imuData->accelerometer.x=accel_event.acceleration.x;
	imuData->accelerometer.y=accel_event.acceleration.y;
	imuData->accelerometer.z=accel_event.acceleration.z;

    
	imuData->magnetometer.x=mag_event.magnetic.x;
	imuData->magnetometer.y=mag_event.magnetic.y;
	imuData->magnetometer.z=mag_event.magnetic.z;

	
	imuData->gyro.x=gyro_event.gyro.x;
	imuData->gyro.y=gyro_event.gyro.y;
	imuData->gyro.z=gyro_event.gyro.z;



	if(0)
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
	if(DBUG)
	{
		Serial.print(F("Heading: "));
		Serial.print(orientation.heading);
		Serial.println(F(""));
	}
		if(0)
	{
		Serial.print("X: "); Serial.print(accel_event.acceleration.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(accel_event.acceleration.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(accel_event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
	}
	if(0){
		Serial.print(F("Acc   "));
		Serial.print("X: "); Serial.print(accel_event.acceleration.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(accel_event.acceleration.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(accel_event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
		
		Serial.print(F("Acc Filt  "));
		Serial.print("X: "); Serial.print(accelFil_event.acceleration.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(accelFil_event.acceleration.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(accelFil_event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
		/* Display the results (magnetic vector values are in micro-Tesla (uT)) */
		
		Serial.print(F("MAG   "));
		Serial.print("X: "); Serial.print(mag_event.magnetic.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(mag_event.magnetic.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(mag_event.magnetic.z); Serial.print("  ");Serial.println("uT");
		
		/* Display the results (gyrocope values in rad/s) */
		
		Serial.print(F("GYRO  "));
		Serial.print("X: "); Serial.print(gyro_event.gyro.x); Serial.print("  ");
		Serial.print("Y: "); Serial.print(gyro_event.gyro.y); Serial.print("  ");
		Serial.print("Z: "); Serial.print(gyro_event.gyro.z); Serial.print("  ");Serial.println("rad/s ");  
		
	}


}
void RR_IMU::updateIMUahrs(void)
{

		accel.getEvent(&accel_event);
		accVec[0]=accel_event.acceleration.x;	accVec[1]=accel_event.acceleration.y;	accVec[2]=accel_event.acceleration.z; 
		mag.getEvent(&mag_event);
		magVec[0]=mag_event.magnetic.x;	magVec[1]=mag_event.magnetic.y;	magVec[2]=mag_event.magnetic.z; 
		gyro.getEvent(&gyro_event);
		gyrVec[0]=gyro_event.gyro.x;	gyrVec[1]=gyro_event.gyro.y;	gyrVec[2]=gyro_event.gyro.z; 

		uimu_ahrs_iterate(gyrVec,accVec,magVec);
		imu::Vector<3> euler = uimu_ahrs_get_euler();
		if(DBUG)
		{
			Serial.print("euler: ");
			Serial.println(euler.x());
			//Serial.print(" ");
			//Serial.print(euler.y());
			//Serial.print(" ");
			//Serial.println(euler.z());
		}


	//imuData->fused.heading=tempavg/avgcount;
	/*
	imuData->fused.roll=orientation.roll;
	imuData->fused.pitch=orientation.pitch;


	imuData->accelerometer.x=accel_event.acceleration.x;
	imuData->accelerometer.y=accel_event.acceleration.y;
	imuData->accelerometer.z=accel_event.acceleration.z;

    
	imuData->magnetometer.x=mag_event.magnetic.x;
	imuData->magnetometer.y=mag_event.magnetic.y;
	imuData->magnetometer.z=mag_event.magnetic.z;

	
	imuData->gyro.x=gyro_event.gyro.x;
	imuData->gyro.y=gyro_event.gyro.y;
	imuData->gyro.z=gyro_event.gyro.z;
	*/

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
