#include "RR_MOSFET.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_MOSFET::RR_MOSFET(void)
{
	pinMode(MOSFET_PIN,OUTPUT);
}

void RR_MOSFET::switchMOSFET(SWITCHMODE _mode)
{
	if(_mode==ON)
	{
		digitalWrite(MOSFET_PIN,HIGH);
	}
	else if (_mode==OFF);
	{
		digitalWrite(MOSFET_PIN,LOW);
	}

}
RR_MOSFET::~RR_MOSFET(void)
{
}
