#include <Servo.h>
#define ESC1_PIN 20
#define ESC2_PIN 21
#define ESC3_PIN 22
#define ESC4_PIN 23

Servo esc1;
Servo esc2;
Servo esc3;
Servo esc4;

void setup()
{
	Serial.begin(56700);
	delay(4000);
	esc1.attach(ESC1_PIN);
	esc2.attach(ESC2_PIN);
	esc3.attach(ESC3_PIN);
	esc4.attach(ESC4_PIN);

	esc1.write(0);
	esc2.write(0);
	esc3.write(0);
	esc4.write(0);
	delay(3000);
	Serial.print("count ");

	Serial.println("Done Arming");
	

}

void loop()
{
	
	esc1.write(80);
	delay(1000);
	esc1.write(10);
	esc2.write(80);
	delay(1000);
	esc2.write(10);
	esc3.write(80);
	delay(1000);
	esc3.write(10);
	esc4.write(80);
	delay(1000);
	esc4.write(10);
	
	
	// Arming Code : It works for some reason!
	/*
	esc4.write(0);
	delay(1000);
	esc4.write(20);
	delay(1000);
	esc4.write(40);
	delay(1000);
	*/
}
