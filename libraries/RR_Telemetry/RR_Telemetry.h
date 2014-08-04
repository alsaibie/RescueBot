#pragma once
#include "../RR_CommonDefines/RR_CommonDefines.h"
#include "../RR_CommonData/RR_CommonData.h"
#include <Arduino.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#define MSG_OUTPACKETSIZE sizeof(RR_TelemetryOutgoingMessage_t)
#define MSG_INPACKETSIZE sizeof(RR_TelemetryIncomingMessage_t)

class RR_Telemetry 
{
  public:  
    // CONSTRUCTORS
	 RR_Telemetry(void);
	 RR_Telemetry(RR_TelemetryOutgoingMessage_t *myOutgoingdata, RR_TelemetryIncomingMessage_t *myIncomingdata);
	 void sendShortMessage(char *str); //To send direct strings for debugging
	 void Update(void);
	 void printMsg(void);
	 //Methods to receive messages
	~RR_Telemetry(void);

  private:
	  void encodeMessage(void); //Packs Message Struct and adds to buffer
	  void transmitOutgoingBuffer(void);
	  void receiveIncomingBuffer(void);
	  bool decodeMessage(void);
	  RR_TelemetryOutgoingMessage_t *telemetryOutMessage;
	  RR_TelemetryIncomingMessage_t *telemetryInMessage;
	  byte *incomingAddress;
	  uint8_t databufferOutgoing[MSG_OUTPACKETSIZE+50];
	  uint8_t databufferIncoming[MSG_INPACKETSIZE+50];
};



