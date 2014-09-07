#include <RR_CommonDefines.h>
#include <RR_CommonData.h>
#include <EEPROM.h>
///////////////////////////////////////////////////////////////////////////////
/*




					PAY ATTENTION! 
					USE THIS CODE TO RESET THE EEPROM







*/
///////////////////////////
//-------------OPTIONS------------
#define USE_EEPROM
uint8_t driveMode=
	MANUAL_PC;
StartMode_t startMode= 
	LAUNCHING;
//---------------------------------

//Global Variables - Ideally each task needs to know the state only, data is exchanged through protected data structs.
MainState_t mainState;
uint8_t * stateaddr = (uint8_t *)0x01;

//Shared Data Structs
void setup()
{
	delay(2000);
	#ifdef USE_EEPROM
	//TEST
	eeprom_write_byte(stateaddr,startMode);
	mainState=(MainState_t)eeprom_read_byte(stateaddr);
	#else
	 mainState=startMode;
	#endif
	 Serial.print("State from EEPROM: ");
	 Serial.println(mainState);
  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}
