#include "RR_Telemetry.h"

// Constructors ////////////////////////////////////////////////////////////////
RR_Telemetry::RR_Telemetry(void){ }

RR_Telemetry::RR_Telemetry(RR_TelemetryOutgoingMessage_t *myOutgoingdata, RR_TelemetryIncomingMessage_t *myIncomingdata)
{
	telemetryOutMessage=myOutgoingdata;
	telemetryInMessage=myIncomingdata;
	incomingAddress=(byte*)&telemetryInMessage;
	Serial3.begin(56700);
}


void RR_Telemetry::encodeMessage(void)
{
   uint8_t CheckSum = MSG_OUTPACKETSIZE;
   uint8_t *outgoingPacketAddress, *outgoingMsgAddress;
   outgoingMsgAddress=(byte*)telemetryOutMessage;
   outgoingPacketAddress=databufferOutgoing;
   (*outgoingPacketAddress)=0x06;
   (*++outgoingPacketAddress)=0x85;
   (*++outgoingPacketAddress)=MSG_OUTPACKETSIZE;

  for(uint8_t i = 0; i<MSG_OUTPACKETSIZE; i++)
  {
    CheckSum^=*(outgoingMsgAddress+i);
    (*++outgoingPacketAddress)=(*(outgoingMsgAddress+i));
  }
  
  (*++outgoingPacketAddress)=CheckSum;
}

void RR_Telemetry::transmitOutgoingBuffer(void)
{
	//For now with just one struct just send the outgoingMSGPacket 
	//but in the future create a buffer and
	//store the outgoing packets in it and send the buffer periodically. 
	//This will be more efficient for multiple messages. 
	Serial3.write(databufferOutgoing,MSG_OUTPACKETSIZE+10);
}
void RR_Telemetry::Update(void)
{
	encodeMessage();
	transmitOutgoingBuffer();

}


void RR_Telemetry::decodeMessage(RR_TelemetryIncomingMessage_t* myIncomingdata)
{
	/*
    int *q = (int*)data;    
	msgPacket->message_id = *q;       q++;    
	msgPacket->sender = *q;   q++;    

    char *p = (char*)q;
    int i = 0;
    while (i < BUFSIZE)
    {
        msgPacket->message[i] = *p;
        p++;
        i++;
    }
	*/
}

void RR_Telemetry::printMsg(RR_TelemetryOutgoingMessage_t* myOutgoingdata)
{
	/*
	Serial.println(myOutgoingdata->message_id);
	Serial.println(myOutgoingdata->sender);
	Serial.println(myOutgoingdata->message);
	*/
}

RR_Telemetry::~RR_Telemetry(void)
{
}
