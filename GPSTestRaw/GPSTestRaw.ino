#define GPSEnable 2
void setup()
{
	Serial.begin(115200);
	delay(2000);
	Serial.println("Test");
	Serial1.begin(9600);
  pinMode(GPSEnable,OUTPUT);
  digitalWrite(GPSEnable,LOW);
  delay(5000);
  digitalWrite(GPSEnable,HIGH);
}
  /* add setup code here */


void loop()
{
	if(Serial1.available())
	{
	Serial.print(Serial1.read());
	}
  /* add main program code here */

}
