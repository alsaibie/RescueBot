#include "DualMC33926MotorShield.h"

DualMC33926MotorShield md;
int _dTH=3;
int TH;
int _dELV=5;
int ELV;

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
  pinMode(_dTH, INPUT); // Set our input pins as such
  pinMode(_dELV, INPUT);
  Serial.begin(115200);
  Serial.println("Dual MC33926 Motor Shield");
  md.init();
  md.setM1Speed(400);
  md.setM2Speed(400);
  delay(3000);
}

void loop()
{
	TH = pulseIn(_dTH, HIGH, 25000); // Read the pulse width of
	md.setM2Speed(TH-1500);
	//md.setM1Speed(400);
	ELV = pulseIn(_dELV, HIGH, 25000); // each channel
	md.setM1Speed(1500-ELV);
	Serial.println(TH);
	Serial.print("M1 current: ");
    Serial.println(md.getM1CurrentMilliamps());
	Serial.print("M2 current: ");
    Serial.println(md.getM2CurrentMilliamps());
    stopIfFault();
	delay(200);
	
	/*
  for (int i = 0; i <= 400; i++)
  {
    md.setM1Speed(i);
    //stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
  for (int i = 400; i >= -400; i--)
  {
    md.setM1Speed(i);
    //stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
  for (int i = -400; i <= 0; i++)
  {
    md.setM1Speed(i);
    //stopIfFault();
    if (abs(i)%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(20);
  }
  
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
