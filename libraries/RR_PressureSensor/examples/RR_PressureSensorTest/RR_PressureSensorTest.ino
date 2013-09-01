#include <TimerOne.h>
#include <Wire.h>
#include <RR_PressureSensor.h>
#include <Adafruit_BMP085.h>
#include <RunningAverage.h>

//

RR_PressureSensor Altimeter;


void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);
	
	Timer1.initialize(1000000);
	TBUG
	Altimeter.Enable();
	
	//
	Timer1.attachInterrupt(Timedloop);
}

void Timedloop(){
Serial.println("This is a Timed Function");
Serial.println(Altimeter.BaseAltitude);
Serial.println(Altimeter.AverageAltitude);

}

void loop() {
	//Serial.println(Altimeter2.readAltitude(ACTUALSEAPRESSURE));
	Altimeter.Update();
	delay(500);
  // put your main code here, to run repeatedly: 
	
}
