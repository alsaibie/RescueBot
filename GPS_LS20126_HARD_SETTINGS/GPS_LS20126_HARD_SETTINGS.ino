/*

Code to Hard Set the GPS device

USE: http://www.hhhh.org/wiml/proj/nmeaxor.html 
to easily calculate checksum (2 hex value after * in message)

*/
#define EnablePin 8

// NMEA MESSAGES
// TO TURN MESSAGES OFF
#define GGA_OFF "$PSRF103,00,00,00,01*24"
#define GLL_OFF "$PSRF103,01,00,00,01*25"
#define GSA_OFF "$PSRF103,02,00,00,01*26"
#define GSV_OFF "$PSRF103,03,00,00,01*27"
#define RMC_OFF "$PSRF103,04,00,00,01*20"
#define VTG_OFF "$PSRF103,05,00,00,01*21"

//TO TURN ON WITH FREQUENCY OF 1HZ
#define GGA_ON_1HZ "$PSRF103,00,00,01,01*25"
#define GLL_ON_1HZ "$PSRF103,01,00,01,01*24"
#define GSA_ON_1HZ "$PSRF103,02,00,01,01*27"
#define GSV_ON_1HZ "$PSRF103,03,00,01,01*26"
#define RMC_ON_1HZ "$PSRF103,04,00,01,01*21"
#define VTG_ON_1HZ "$PSRF103,05,00,01,01*20"


//CHANGE PLSR FREQUENCY TO 5HZ
#define PLSR_ON_5HZ "$PLSC,248,1,2*1D"
	
//CHANGE PLSR FREQUENCY TO 1HZ
#define PLSR_ON_1HZ "$PLSC,248,0*0F"

void setup()
{
   pinMode(8, OUTPUT); 
   digitalWrite(8, HIGH);
   Serial.begin(9600);
   delay(200);
   Serial.println(GGA_OFF);
   delay(200);
   Serial.println(GLL_OFF);
   delay(200);
   Serial.println(GSA_OFF);
   delay(200);
   Serial.println(GSV_OFF);
   delay(200);
   Serial.println(VTG_OFF);
   delay(200);
   Serial.println(PLSR_ON_5HZ);
   delay(200);
   Serial.println("Test Outcome");
   
}

void loop()
{
  
  
  if (Serial.available())
	{
              char c=Serial.read();
              Serial.write(c);
        }
}
