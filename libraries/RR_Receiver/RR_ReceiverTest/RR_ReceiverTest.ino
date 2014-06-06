
#include <RR_Receiver.h>
#include <RR_CommonDefines.h>

#define _dTHR THRO_RX_PIN
#define _dAIL AILE_RX_PIN
#define _dELE ELEV_RX_PIN
#define _dRUD RUDD_RX_PIN

int THR, AIL, ELE, RUD;
RR_Receiver Spektrum;
void setup()
{
	/*
  pinMode(_dTHR, INPUT); // Set our input pins as such
  pinMode(_dAIL, INPUT);
  pinMode(_dELE, INPUT);
  pinMode(_dRUD, INPUT);*/
  Serial.begin(115200);
  Serial.println("Receiver Test");
  delay(3000);
  /* add setup code here */

}

void loop()
{ 
	Serial.println(Spektrum.getSignal(AILE));
	/*
  THR = pulseIn(_dTHR, HIGH, 25000); // Read the pulse width of
  Serial.println(THR);
  AIL = pulseIn(_dAIL, HIGH, 25000); // Read the pulse width of
  Serial.println(AIL);
  ELE = pulseIn(_dELE, HIGH, 25000); // Read the pulse width of
  Serial.println(ELE);
  RUD = pulseIn(_dRUD, HIGH, 25000); // Read the pulse width of
  //Serial.println(RUD);
  
	*/
}
