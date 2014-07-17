#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
	uint8_t msg_timestamp; //time
	uint8_t launchState;
	uint8_t navigationalState;
	uint8_t latitude;
	uint8_t longitude;
	uint8_t distanceToTarget;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial3.begin(56700);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial3);
  mydata.latitude = 50;
  mydata.longitude = 20;
   //mydata.size=sizeof(mydata);
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));
  
}

void loop(){
  //this is how you access the variables. [name of the group].[variable name]
 //random(5);
  //mydata.pause = random(5);
  //send the data
  ET.sendData();
  /*
  //Just for fun, we will blink it out too
   for(int i = mydata.blinks; i>0; i--){
      digitalWrite(13, HIGH);
      delay(mydata.pause * 100);
      digitalWrite(13, LOW);
      delay(mydata.pause * 100);
    }
  */
 // Serial.println(mydata.size);
  delay(2000);
}
