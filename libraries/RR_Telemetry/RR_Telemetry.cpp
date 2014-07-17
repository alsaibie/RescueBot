#include "RR_Telemetry.h"

// Constructors ////////////////////////////////////////////////////////////////
RR_Telemetry::RR_Telemetry(void){ }

RR_Telemetry::RR_Telemetry(RR_TelemetryOutgoingMessage_t *myOutgoingdata, RR_TelemetryIncomingMessage_t *myIncomingdata)
{
	telemetryOutMessage=myOutgoingdata;
	telemetryInMessage=myIncomingdata;
	incomingAddress=(byte*)telemetryInMessage;
	telemetryInMessage->Joystick.Pad_Left.Y_Axis=0;
	Serial3.begin(56700);
}


bool RR_Telemetry::decodeMessage(void){
	if(Serial3.available() >= 3){
		while(Serial3.read() != 0x06){
			if(Serial3.available() < 3)
				return false;
		}
		if(Serial3.read() != 0x85){
			if(DBUG) {Serial.println("Header Doesn't Match");}
			return false;
		}
		if(Serial3.read()!=MSG_INPACKETSIZE) {
			if(DBUG) {Serial.println("Message Received Size Don't Match");}
			return false;
			}	
		int receive_array_ind=0;
		while (Serial3.available() && receive_array_ind<=MSG_INPACKETSIZE){
			databufferIncoming[receive_array_ind++] = Serial3.read();
		}
		if((receive_array_ind-1)!=MSG_INPACKETSIZE) {
			if(DBUG) {Serial.println("Message Buffer Not Same Size");}
			return false;
			}
		int inCS=MSG_INPACKETSIZE;
		for (int i=0; i<MSG_INPACKETSIZE; i++){
			inCS^=databufferIncoming[i];
		}
		if(inCS != databufferIncoming[receive_array_ind-1]){
			if(DBUG) {Serial.println("Checksum bad :(");}
			return false;
		}
		else{	
		if(DBUG) {Serial.println("Checksum Good :D");}
		}
		memcpy(incomingAddress, databufferIncoming, MSG_INPACKETSIZE);
		return true;
		if(DBUG){
		}

	}

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
	if(decodeMessage())
		{
			if(DBUG){
				Serial.print("Joystick: ");
				Serial.println(telemetryInMessage->Joystick.Pad_Right.Y_Axis);
			}
	}
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
