#include <RR_Servo.h>
#include <Servo.h>
#include <RR_CommonDefines.h>

RR_Servo servo;
int pos=0;
void setup() {

	servo.Enable();

	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);
	Serial.println("Ready");

}

void loop() {

	for( pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
		{                                  // in steps of 1 degree 
			servo.setAngle(pos);              // tell servo to go to position in variable 'pos' 
		delay(15);                       // waits 15ms for the servo to reach the position 
		} 
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
		  {                                
			  servo.setAngle(pos);              // tell servo to go to position in variable 'pos' 
			delay(15);                       // waits 15ms for the servo to reach the position 
		  } 
  
}
