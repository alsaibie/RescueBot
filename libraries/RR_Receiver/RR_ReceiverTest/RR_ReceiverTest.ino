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
  Serial.begin(115200);
  Serial.println("Receiver Test");
  delay(3000);
}

void loop()
{ 
	Serial.print(F("Throttle:   "));
	Serial.println(Spektrum.getSignal(THRO));
	Serial.print(F("Aileron:    "));
	Serial.println(Spektrum.getSignal(AILE));
	Serial.print(F("Elevation:  "));
	Serial.println(Spektrum.getSignal(ELEV));
	delay(100);
}
