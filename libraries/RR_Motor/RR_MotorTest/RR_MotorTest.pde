#include <RR_CommonDefines.h>
#include <RR_Motor.h>

RR_Motor md;

// Choose 
#define M1Test 1
#define M2Test 1


void stopIfFault()
{
  if (md.getFault())
  {
    Serial.println("fault");
    while(1);
  }
}

void setup()
{
  Serial.begin(115200);
  delay(3000); // Delay to see output on serial
  Serial.println("RR_Motor Test");
  md.init();
  md.setM1Speed(400);
  //md.setM2Speed(400);
}

void loop()
{
	// Accelerate Forward Motor 1 
	/*
  for (int i = 0; i <= 400; i++)
  {
    md.setM1Speed(i);
   // stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
  delay(2000); //Hold speed
  
  //Sweep backward
  for (int i = 400; i >= -400; i--)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
  delay(2000); //hold speed
  
  //Sweep to Neutral
  for (int i = -400; i <= 0; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
  // Accelerate Forward Motor 2
  for (int i = 0; i <= 400; i++)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(20);
  }

  delay(2000); //Hold speed
  
  //Sweep backward
  for (int i = 400; i >= -400; i--)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(20);
  }
  delay(2000); //hold speed
  
  //Sweep to Neutral
  for (int i = -400; i <= 0; i++)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(20);
  }
  */
}
