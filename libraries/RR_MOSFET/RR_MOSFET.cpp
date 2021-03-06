#include "RR_MOSFET.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_MOSFET::RR_MOSFET(void)
{
	pinMode(MOSFET_PIN,OUTPUT);
}

void RR_MOSFET::switchMOSFET(SwitchMode_t _mode)
{
	if(_mode)
	{
		digitalWrite(MOSFET_PIN,HIGH);
	}
	else
	{
		digitalWrite(MOSFET_PIN,LOW);
	}

}
RR_MOSFET::~RR_MOSFET(void)
{
}
