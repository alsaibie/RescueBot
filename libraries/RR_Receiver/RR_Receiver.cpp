#include "RR_Receiver.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_Receiver::RR_Receiver(void)
{
  pinMode(_dTHR, INPUT);
  pinMode(_dAIL, INPUT);
  pinMode(_dELE, INPUT);
  pinMode(_dRUD, INPUT);
}

int RR_Receiver::getSignal(CHANNEL _chan)
{
	if (_chan==THRO)
	return pulseIn(_dTHR, HIGH, 25000);
	else if (_chan==AILE)
	return pulseIn(_dAIL, HIGH, 25000);
	else if (_chan==ELEV)
	return pulseIn(_dELE, HIGH, 25000);
	else if (_chan==RUDD)
	return pulseIn(_dRUD, HIGH, 25000);
}