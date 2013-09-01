
#include <EEPROM.h>
#include <RR_CommonDefines.h>

int EEPROMadr=0;

void setup()
{
	Serial.begin(9600);
	for (int k=0;k<5;k++)
	{
		//EEPROM.write(EEPROMadr,k);
		Serial.println(EEPROM.read(EEPROMadr));
		delay(100);
	}

}

void loop()
{

  /* add main program code here */

}
