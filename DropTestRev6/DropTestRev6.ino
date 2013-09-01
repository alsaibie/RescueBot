/*

Example Code for 3-Axis Accelerometer MMA7361L 

WARNING: Don't connect the logic pins to the Arduino 5V PIN volts. The board only works with 3.3V 

Created by Ali AlSaibie 2013.06.05

*/
#include <Adafruit_BMP085.h>
#include <RunningAverage.h>
#include <Wire.h>
#include <string.h>
#include <ctype.h>

//Sensors can be turned off by changing the macro values - 0 (OFF) would automatically disable the sensor/s related code sections below.
#define PRESSUREREAD 1		// BAROMETRIC PRESSURE SENSOR (I2C Type connected to pin 5-SCL 4-SDA, 3.3V VCC)
#define ACCMTRREAD 1		// ACCELEROMETER (Analog Type)
#define GPSREAD 1			// GPS (Serial Type Connected to RX) 

//PRESSURE SENSOR GLOBRAL VARIABLES
#if(PRESSUREREAD)
Adafruit_BMP085 PSensor;
RunningAverage PSensorAvg(10);
int ps_delay=20; //ms delays to grab reading from pressure sensor
int pl_delay=100; //ms delay to display/record pressure readings
double ActualSeaLevelPressure=101500; //Change when the accurate value is known. Perhaps compute pressure in relative terms. (relative to referenced site ground altitude)
char bufferPressure[10]= " ";
unsigned long time_ps, time_pl, time_psd, time_pld;
#endif

//ACCELEROMETER GLOBAL VARIABLES
#if(ACCMTRREAD)

#define ACCSELECT	1 //0 for 1.5g range, 1 for 6g range - would determine V-to-g conversion parameters
#define RECG		0 //To switch between recording voltage or converting to g and recording 
const int AnalogInDelay=2; //To set delay time after reading analog in - helps settle voltage reading (CONFIRM)

//Accelerometer ADC Input Pins
int XPin=1;
int YPin=2;
int ZPin=3;

//Reference Voltage - By default 5Volts - Not changed.
int Vref=5;

const int AccAverageSize=5;
RunningAverage XgAverage(AccAverageSize);
RunningAverage YgAverage(AccAverageSize);
RunningAverage ZgAverage(AccAverageSize);
//double XgAverage, Xg[AccAverageSize];
//double YgAverage, Yg[AccAverageSize];
//double ZgAverage, Zg[AccAverageSize];

int as_delay=20; //ms delay to grab reading from accelerometer readings
int al_delay=100; //ms delay to display/record accelerometer readings

//Timing Services Variables
unsigned long time_as, time_al, time_asd, time_ald;

#if(ACCSELECT)
double XAccRate=4;
double XAccOffset=-7;
double YAccRate=4.8;
double YAccOffset=-8.7;
double ZAccRate=4.25;
double ZAccOffset=-7.38;
#else
double XAccRate=1.25;
double XAccOffset=-1.94;
double YAccRate=1.3;
double YAccOffset=-2.12;
double ZAccRate=1.31;
double ZAccOffset=-1.77;
#endif
#endif

//GPS GLOBAL VARIABLES
#if(GPSREAD)
int byteGPS=-1;
char buffer[300] = "";
char GPGGACode[7] = "$GPGGA";
char PLSRCode[12] = "$PLSR,245,1"; 
int codeCheck=0;
int codeCheck2=0;
int bufferIndex=0;
int byteGPScount=0;
#endif

/////////////////////////// MOVING AVERAGE FUNCTION /////////////////////////////////
double getMovingAverage(double arr[], double vrr)
{
	int i;
	double sum = 0;       
	double avg;          
	int size=sizeof(arr);

	for (i = 0; i < size-1; i++)
	{
		arr[i]=arr[i+1];
		sum += arr[i];
	}
	arr[i]=vrr;
	sum += arr[i];

	avg = (double)sum / size;
	return avg;
}

//////////////////////////////////// SETUP /////////////////////////////////////////////
void setup()
{
	//COMMON SETUP
	Serial.begin(9600);

	//PRESSURE SENSOR SETUP
#if(PRESSUREREAD)
	if (!PSensor.begin()) {
		Serial.println("Check connection to BMP085 Sensor");
		while (1) {}
	}
	PSensorAvg.clear();
	time_ps=millis(); time_pl=millis();
#endif

	//ACCELEROMETER SETUP
#if(ACCMTRREAD)
	time_as=millis(); time_al=millis();
#endif

	//GPS SETUP
#if(GPSREAD) 
	pinMode(6, OUTPUT); 
	digitalWrite(6, HIGH); //enable gps
#endif	
}



void loop()
{
	///////////////////////// PRESSURE SENSOR /////////////////////
	///////////////////////////////////////////////////////////////

#if(PRESSUREREAD)

	//Averaging pressure readings
	time_psd=millis()-time_ps;
	if (time_psd>=ps_delay)
	{
		time_ps=millis();
		PSensorAvg.addValue(PSensor.readAltitude(ActualSeaLevelPressure));

	}

	time_pld=millis()-time_pl;
	if (time_pld>=pl_delay)
	{		
		time_pl=millis();
		Serial.write("$ALT,"); Serial.print(time_pld);
		Serial.write(","); Serial.print((int)(PSensorAvg.getAverage()*10));
		Serial.write("\n"); 

	}
#endif

	///////////////////////// ACCELEROMETER ///////////////////////
	///////////////////////////////////////////////////////////////

#if(ACCMTRREAD)

	//XgAverage=getMovingAverage(Xg,((double)analogRead(XPin)/1024*Vref));
	////Serial.println(XgAvg);
	//YgAverage=getMovingAverage(Yg,((double)analogRead(YPin)/1024*Vref));
	////Serial.println(YgAvg);
	//ZgAverage=getMovingAverage(Zg,((double)analogRead(ZPin)/1024*Vref));
	//Serial.println(ZgAvg);


	time_asd=millis()-time_as;
	if (time_asd>=as_delay)
	{
		time_as=millis();
		if (RECG) //Get g using calibrated offsets - g select at top
		{
			XgAverage.addValue(((double)analogRead(XPin)/1024*Vref)*XAccRate+XAccOffset);
			//delay(AnalogInDelay);
			YgAverage.addValue(((double)analogRead(YPin)/1024*Vref)*YAccRate+YAccOffset);
			//delay(AnalogInDelay);
			ZgAverage.addValue(((double)analogRead(ZPin)/1024*Vref)*ZAccRate+ZAccOffset);
			//delay(AnalogInDelay);
		}
		else //Get Voltage Directly
		{
			XgAverage.addValue(((double)analogRead(XPin)/1024*Vref));
			//delay(AnalogInDelay);
			YgAverage.addValue(((double)analogRead(YPin)/1024*Vref));
			//delay(AnalogInDelay);
			ZgAverage.addValue(((double)analogRead(ZPin)/1024*Vref));
			//delay(AnalogInDelay);
		}		
	}

	time_ald=millis()-time_al;
	if (time_ald>=al_delay)
	{
		time_al=millis();
		//Serial.write(bufferAcc);
		Serial.write("$ACC,"); Serial.print(time_ald);
		Serial.write(","); Serial.print((int)(XgAverage.getAverage()*100));
		Serial.write(","); Serial.print((int)(YgAverage.getAverage()*100)); 
		Serial.write(","); Serial.print((int)(ZgAverage.getAverage()*100));
		Serial.write("\n"); 
	}


#endif

	//////////////////////////// GPS //////////////////////////////
	///////////////////////////////////////////////////////////////

#if(GPSREAD) 

	if (Serial.available())
	{

		byteGPS=Serial.read();         // Read a byte of the serial port
		//Serial.write(byteGPS);
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
		//    

	}  
#endif



}

