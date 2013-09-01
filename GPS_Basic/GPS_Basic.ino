#include <string.h>
#include <ctype.h>

int byteGPS=-1;
char buffer[300] = "";
char GPGGACode[7] = "$GPGGA";
char PLSRCode[12] = "$PLSR,245,1"; 
int codeCheck=0;
int codeCheck2=0;
int bufferIndex=0;
int byteGPScount=0;

void setup() {
        Serial.begin(9600);
        pinMode(8, OUTPUT); 
	digitalWrite(8, HIGH); //enable gps  // put your setup code here, to run once:
      Serial.println("Reset");
}

void loop() {
  
if (Serial.available())
	{
              char c=Serial.read();
              Serial.write(c);
           
		//byteGPS=Serial.read();         // Read a byte of the serial port
		//Serial.write(byteGPS);
		
/*
                if (byteGPS == -1) {           // See if the port is empty yet
			delay(100); 
		} else {
			buffer[bufferIndex]=byteGPS;        // If there is serial port data, it is put in the buffer
			bufferIndex++;                      

			if (byteGPS==13){            // If the received byte is = to 13, end of transmission line

				codeCheck=0;
				codeCheck2=0;

				for (int i=0;i<6;i++){     // Verifies if the received command

					if (buffer[i+1]==GPGGACode[i]){
						codeCheck++;
					}
				}

				for (int i=0;i<11;i++){     // Verifies if the received command

					if (buffer[i+1]==PLSRCode[i]){
						codeCheck2++;
					}
				}

				if(codeCheck==6||codeCheck2==11){               
					for(int i=0;i<300;i++){
						if(buffer[i]!=32){
							Serial.write(buffer[i]);
						}
					}
					Serial.write("\n");
				} //end if codeCheck true

				bufferIndex=0;                    // Reset the buffer
				for (int i=0;i<300;i++){    //  
					buffer[i]=' ';             
				}                 
			}
		}
		//    
		//    */

	}  

  
}
