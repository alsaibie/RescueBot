#define _dTHR 3
#define _dSTR 4
int THR, STR;

void setup()
{
  pinMode(_dTHR, INPUT); // Set our input pins as such
  pinMode(_dSTR, INPUT);
  Serial.begin(115200);
  Serial.println("Receiver Test");
  delay(3000);
  /* add setup code here */

}

void loop()
{ 
  THR = pulseIn(_dTHR, HIGH, 25000); // Read the pulse width of
  Serial.println(THR);
  STR = pulseIn(_dSTR, HIGH, 25000); // Read the pulse width of
  Serial.println(STR);
  /* add main program code here */

}
