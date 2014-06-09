#include "RR_Receiver.h"

// Constructors ////////////////////////////////////////////////////////////////

RR_Receiver::RR_Receiver(void)
{
  pinMode(THRO_RX_PIN, INPUT);
  pinMode(AILE_RX_PIN, INPUT);
  pinMode(ELEV_RX_PIN, INPUT);
  pinMode(RUDD_RX_PIN, INPUT);
}

int RR_Receiver::getSignal(CHANNEL _chan)
{
	if (_chan==THRO)
	return pulseIn(THRO_RX_PIN, HIGH, 25000);
	else if (_chan==AILE)
	return pulseIn(AILE_RX_PIN, HIGH, 25000);
	else if (_chan==ELEV)
	return pulseIn(ELEV_RX_PIN, HIGH, 25000);
	else if (_chan==RUDD)
	return pulseIn(RUDD_RX_PIN, HIGH, 25000);
}