#include <RR_CommonDefines.h>
#include <RR_Motor.h>

RR_Motor motor;
void setup() {
	motor.Enable();
  // put your setup code here, to run once:

}

void loop() {
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



  // put your main code here, to run repeatedly: 
  
}
