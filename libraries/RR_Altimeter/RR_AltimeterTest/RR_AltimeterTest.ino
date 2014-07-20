#include <Adafruit_BMP085_U.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <RR_Altimeter.h>

RR_AltimeterData_t altimeterData;
RR_Altimeter altimeter(&altimeterData);
void setup()
{  
	altimeter.initAltimeter();
	delay(2000);
	altimeter.DisplayAltimeterDetails();
	delay(2000);
}

void loop()
{
	altimeter.updateAltimeter();
}
