#define HWSERIAL Serial1

void setup() {
	pinMode(2,OUTPUT);
	digitalWrite(2,HIGH);
	Serial.begin(9600);
    HWSERIAL.begin(115200);
}

void loop() {
    int incomingByte;
        
	if (Serial.available() > 0) {
		incomingByte = Serial.read();
		Serial.print("USB received: ");
		Serial.println(incomingByte, DEC);
        HWSERIAL.print("USB received:");
        HWSERIAL.println(incomingByte, DEC);
	}
	if (HWSERIAL.available() > 0) {
		incomingByte = HWSERIAL.read();
		Serial.print("UART received: ");
		Serial.println(incomingByte, DEC);
        HWSERIAL.print("UART received:");
        HWSERIAL.println(incomingByte, DEC);
	}
}