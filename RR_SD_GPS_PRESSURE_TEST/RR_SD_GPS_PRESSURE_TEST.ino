#include <Servo.h>
#include <RR_Motor.h>
#include <RR_Servo.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <RunningAverage.h>
#include <RR_SDCard.h>
#include <RR_PressureSensor.h>
#include <RR_GPS.h>
#include <RR_CommonDefines.h>
#include <SoftwareSerial.h>



RR_GPS gps;
//RR_PressureSensor Altimeter;
RR_Motor motor;
RR_Servo servo;
RR_SDCard SD;

int recordcount=0;
long time1;
char Altitude[50];
char Coordinates[50];

void setup()
{
	//delay(2000);
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println(F("Hi This is a Data Logging test for BMP085+GPS"));

	//Serial.println(F("Initializing Timer"));
	//Timer1.initialize(400000);
	//Serial.println(F("Timer initialized"));
	
	Serial.println(F("Enabling GPS"));
	delay(100);
	gps.Enable();
	Serial.println(F("GPS Enabled"));
	
	delay(100);
	/*
	Serial.println(F("Enabling Pressure Sensor"));
	Altimeter.Enable();
	Serial.println(F("Pressure Sensor Enabled"));
	delay(1000);
	*/

	Serial.println(F("Initializing SD Card"));
	SD.Initialize();
	Serial.println(F("SD Card Initiazlied"));
	delay(400);
	//Timer1.attachInterrupt(record);
	Serial.print(F("Free SRAM Remaining: "));
	Serial.println(freeRam());
	SD.Write("test SD at Setup");
	delay(300);


	Serial.println(F("Initializing Motor"));
	motor.Enable();
	Serial.println(F("Motor Initiazlied"));
	delay(50);
	Serial.println(F("Initializing Servo"));
	servo.Enable();
	Serial.println(F("Servo Initiazlied"));

}

void loop()
{
	//altimeterloop();

	gpsloop();
	
	delay(300);

	SDloop();
	
	delay(100);

	motorloop();

	servoloop();
	/*
	if (recordcount<10)
	{
		recordcount++;
		if (recordcount==10)
		{
			Altimeter.Disable();
		}
	}
	*/
}

void SDloop()
{
//	SD.WriteMessage(Altimeter.AverageAltitude,ALTITUDE_MSG);
	SD.WriteMessage(gps.longitude, COORDINATE_MSG);

}


void gpsloop()
{
	time1=millis();
	Serial.println(F("GPS loop"));
	gps.readMessage();
	//gps.displayData(GPRMC);   // This can be used to display all the data of the data from GPRMC report
	//gps.displayData(PLSR);    // This can be used to display all the data of the data from PLSR report
	//sprintf(Altitude,"Altitude: %f",Altimeter.AverageAltitude);
	//sprintf(Coordinates,"Coordinates: %f, %f",gps.latitude,gps.longitude);
	//Serial.println(gps.course);
	Serial.print(F("Time: "));
	Serial.println(millis()-time1);
}

/*
void altimeterloop()
{
	Serial.println(F("Altimeter loop"));
	Altimeter.Update();
	Serial.println(Altimeter.AverageAltitude);
	Serial.print(F("Time: "));
	Serial.println(millis()-time1);
}

*/
void motorloop()
{
	time1=millis();
	Serial.println(F("Motor loop"));
	
	for (int i=0; i<255; i++)
	{
		motor.Speed(i,1);
		//analogWrite(MOTOR1_ENABLE_PIN,i);
		delay(20);
	}

	delay(500);

	for (int i=255; i>0; --i)
	{
		motor.Speed(i,1);
		//analogWrite(MOTOR1_ENABLE_PIN,i);
		delay(20);
	}

	delay(500);

	for (int i=0; i<255; i++)
	{
		motor.Speed(i,0);
		//analogWrite(MOTOR1_ENABLE_PIN,i);
		delay(20);
	}

	delay(500);

	for (int i=255; i>0; --i)
	{
		motor.Speed(i,0);
	//	analogWrite(MOTOR1_ENABLE_PIN,i);
		delay(20);
	}
	Serial.print(F("Time: "));
	Serial.println(millis()-time1);
}

void servoloop()
{
	time1=millis();
	Serial.println(F("servo loop"));

	for( int pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
		{                                  // in steps of 1 degree 
			servo.setAngle(pos);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
		} 
  for(int pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
		  {                                
			  servo.setAngle(pos);              // tell servo to go to position in variable 'pos' 
			delay(15);                       // waits 15ms for the servo to reach the position 
		  } 
	Serial.print(F("Time: "));
	Serial.println(millis()-time1);
}


/*
void record()
{
	
	//delay(2000);
	//gps.readMessage();            // This will read the GPS and collect the GPRMC and PLSR report and update the variables
	
	
	//delay(600);
	//Serial.println("Record");
	//gps.displayData(GPRMC);   // This can be used to display all the data of the data from GPRMC report
	//gps.displayData(PLSR);    // This can be used to display all the data of the data from PLSR report
	//Serial.print("direction: ");
	//Serial.println(gps.dir);  // The individual variables can also be accessed for manipulation
	//Serial.println(gps.latitude,3);
	//sprintf(Altitude,"Altitude: %f",Altimeter.AverageAltitude);
	//sprintf(Coordinates,"Coordinates: %f, %f",gps.latitude,gps.longitude);
	//SD.Write(Altitude,ALTITUDE_MSG);
	//Serial.println(Altimeter.AverageAltitude,5);
	//SD.Write(Coordinates,COORDINATE_MSG);
	
}
*/


int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}