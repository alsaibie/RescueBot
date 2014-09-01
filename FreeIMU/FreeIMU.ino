#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
float mag_x_offset = 1.225;
float mag_y_offset = -8.144;
float mag_z_offset = -1.236;
float mag_x_scale = 35.733;
float mag_y_scale = 45.266;
float mag_z_scale = 41.427;
/* Assign a unique ID to the sensors */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

void displaySensorDetails(void)
{
  sensor_t sensor;
  
  accel.getSensor(&sensor);

  gyro.getSensor(&sensor);
 
  mag.getSensor(&sensor);

  bmp.getSensor(&sensor);
 
  delay(500);
}

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Adafruit 10DOF Tester")); Serial.println("");
  
  /* Initialise the sensors */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(3000);
  /* Display some basic information on this sensor */
  displaySensorDetails();
  delay(5000);
}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
   
  /* Display the results (acceleration is measured in m/s^2) */
  accel.getEvent(&event);
 // Serial.print(F("ACCEL "));
  Serial.print(event.acceleration.x,8); Serial.print(",");
  Serial.print(event.acceleration.y,8); Serial.print(",");
  Serial.print(event.acceleration.z,8); Serial.print(",");
  
  /* Display the results (gyrocope values in rad/s) */
 // gyro.getEvent(&event);
 // Serial.print(event.gyro.x,8); Serial.print(",");
 // Serial.print(event.gyro.y,8); Serial.print(",");
 // Serial.print(event.gyro.z,8); Serial.println("");
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  mag.getEvent(&event);
  //Serial.print((event.magnetic.x-1.95844616)/33.1688,8); Serial.print(",");
  if(0){
  Serial.print(event.magnetic.x,8); Serial.print(",");
  Serial.print(event.magnetic.y,8); Serial.print(",");
  Serial.print(event.magnetic.z,8); 
  Serial.println("");
  }
	if(1){
	Serial.print((event.magnetic.x-mag_x_offset)/mag_x_scale,8); Serial.print(",");
	Serial.print((event.magnetic.y-mag_y_offset)/mag_y_scale,8); Serial.print(",");
	Serial.print((event.magnetic.z-mag_z_offset)/mag_z_scale,8);
	Serial.println("");
  }

  /*

  // Display the pressure sensor results (barometric pressure is measure in hPa) 
  bmp.getEvent(&event);
  if (event.pressure)
  {
    // Display atmospheric pressure in hPa 

    Serial.print(event.pressure); Serial.print("	");

    // Display ambient temperature in C 
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print(temperature); Serial.print("	");
    // Then convert the atmospheric pressure, SLP and temp to altitude    
    // Update this next line with the current SLP for better results      
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
   // Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                  //                      event.pressure,
                    //                    temperature)); 
    //Serial.println(F(" m"));

	
  }
  */
 // Serial.println(F(""));
  delay(100);
}