void setup()
{
	delay(2000); //For Teensy Virtual COM to catch up and display on Serial Monitor
	Serial.begin(57600);
	Serial.println("Hi! I'm testing 3DR Telemetry");

	Serial3.begin(57600);
	Serial.println("Connected to 3DR Telemetry");
	
}

void loop()
{

	if (Serial3.available())
    Serial.write(Serial3.read());
	if (Serial.available())
    Serial3.write(Serial.read());
  /* add main program code here */

}
