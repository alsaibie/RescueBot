void setup()
{
	Serial.begin(56700);

  /* add setup code here */

}

void loop()
{
	Serial.println("Hi This is Teensy");
	Serial.println(12345);
	delay(2000);
  /* add main program code here */
}
