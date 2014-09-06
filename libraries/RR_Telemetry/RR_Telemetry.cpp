#include "RR_Telemetry.h"

// Constructors ////////////////////////////////////////////////////////////////
RR_Telemetry::RR_Telemetry(void){ }

RR_Telemetry::RR_Telemetry(RR_TelemetryOutgoingMessage_t *myOutgoingdata, RR_TelemetryIncomingMessage_t *myIncomingdata)
{
	telemetryOutMessage=myOutgoingdata;
	telemetryInMessage=myIncomingdata;
	incomingAddress=(byte*)telemetryInMessage;
	//incomingAddress=(uint8_t)&(*telemetryInMessage);
	sizeIncoming=sizeof(*telemetryInMessage);
	databufferIncoming = (byte*) malloc(sizeIncoming);
	Serial3.begin(56700);
}


bool RR_Telemetry::decodeMessage(void){
	//if(DBUG) {Serial.println("Test");}
	//TBUG
	if(Serial3.available() >= 3){
							
		while(Serial3.read() != 0x06){
			if(Serial3.available() < 3)
				return false;
		}
		
		if(Serial3.read() != 0x85){
			if(DBUG) {Serial.println("Header Doesn't Match");}
			return false;
		}
		if(Serial3.read()!=sizeIncoming) {
			if(DBUG) {Serial.println("Message Received Size Don't Match");}
			return false;
			}	
		uint16_t receive_array_ind=0;
		
		while (Serial3.available() && receive_array_ind<=sizeIncoming){
		//while (receive_array_ind<=sizeIncoming){
				databufferIncoming[receive_array_ind++] = Serial3.read();
				delay(2);
		}
		if((receive_array_ind-1)!=sizeIncoming) {
			if(DBUG){
				Serial.print("Struct Size: "); Serial.print(sizeIncoming);
				Serial.print(" ,Received: "); Serial.println(receive_array_ind-1);
			}
			if(DBUG) {Serial.println("Message Buffer Not Same Size");}
			return false;
			}
		int inCS=sizeIncoming;
		for (int i=0; i<sizeIncoming; i++){
			inCS^=databufferIncoming[i];
		}
		if(inCS == databufferIncoming[receive_array_ind-1]){
			if(0) {Serial.println("Checksum Good :D");}
		
			//memcpy(incomingAddress, databufferIncoming,MSG_INPACKETSIZE);
			memcpy(incomingAddress, databufferIncoming, sizeIncoming);
			receive_array_ind=0;
			if(DBUG){
				//printMsg();
			}
		return true;
		}
		else{
			if(DBUG) {Serial.println("Checksum bad :(");}
			return false;
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
	}
}



void RR_Telemetry::printMsg(void)
{

	Serial.print("Joystick Left X: ");
	Serial.println(telemetryInMessage->Joystick.Pad_Left.X_Axis);
	Serial.print("Joystick Left Y: ");
	Serial.println(telemetryInMessage->Joystick.Pad_Left.Y_Axis);
	Serial.print("Joystick Right X: ");
	Serial.println(telemetryInMessage->Joystick.Pad_Right.X_Axis);
	Serial.print("Joystick Right Y: ");
	Serial.println(telemetryInMessage->Joystick.Pad_Right.Y_Axis);
	Serial.print("StartState: ");
	Serial.println(telemetryInMessage->StartState);
	Serial.print("DriveMode: ");
	Serial.println(telemetryInMessage->DriveMode);
	
}
bool RR_Telemetry::SendSettingCommand(const char  *str){
	Serial3.write("+++");
	if(DBUG){
		while(Serial3.available()){
			Serial.print(Serial3.read());
		}
	}
	delay(1100);
	Serial3.write(str);
	if(DBUG){
		while(Serial3.available()){
			Serial.print(Serial3.read());
		}
	}
	delay(1000);
	Serial3.write("ATO");
	if(DBUG){
		while(Serial3.available()){
			Serial.print(Serial3.read());
		}
	}
	return 1;
}

RR_Telemetry::~RR_Telemetry(void)
{
}
