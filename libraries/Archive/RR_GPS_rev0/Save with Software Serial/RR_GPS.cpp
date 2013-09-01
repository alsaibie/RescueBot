/********************************************************************
* GPS.cpp - Libary for using the GPS module - LS20126 from Sparkfun *
*********************************************************************/

#if defined(ARDUINO) && ARDUINO >= 100
// Choose Arduino.h for IDE 1.0
#include "Arduino.h"
#else
// Choose WProgram.h if IDE is older than 1.0
#include "WProgram.h"
#endif
#include "RR_GPS.h"

char byteGPS      = -1;
char buffer[300]  = "";
int  bufferIndex  = 0;


RR_GPS::RR_GPS(void):gps_SoftSerial(GPS_RX_PIN,GPS_TX_PIN)
{
	//_baud = baud;
	
	pinMode(GPS_ENABLE_PIN,OUTPUT);
	_new_term=false;
	
}

void RR_GPS::Enable()
{
	digitalWrite(GPS_ENABLE_PIN,HIGH);
	gps_SoftSerial.begin(9600);
	gps_SoftSerial.println(GGA_OFF);
    delay(200);
    gps_SoftSerial.println(GLL_OFF);
    delay(200);
    gps_SoftSerial.println(GSA_OFF);
    delay(200);
    gps_SoftSerial.println(GSV_OFF);
    delay(200);
    gps_SoftSerial.println(VTG_OFF);
	while(gps_SoftSerial.available())
	{
		if(gps_SoftSerial.read() == '$')
			break;
	}
	
}

int RR_GPS::readMessage()
{
	char GPRMCBuffer[300] = "", PLSRBuffer[300] = "";
	while(1)
	{
		
		if(gps_SoftSerial.available())
		{		
			byteGPS = gps_SoftSerial.read();                  // Read a byte of the serial port
			if(byteGPS == -1)
			{                        // Wait 100ms if Port empty
				return 0; 
			}
			else
			{
				if(byteGPS=='$')
				{
					_new_term=true;
					_term_type=_TERM_OTHER;	
					
				}

				else if (_new_term) // $ sign not carried to buffer
				{
					switch(_term_type)
					{
					case _TERM_OTHER:	//New Term. Store enough characters to determine if of interest.
						buffer[bufferIndex] = byteGPS;        // Store valid Serial data in Buffer
						bufferIndex++;
						
						if (bufferIndex==10)
						{	
							
							if(strncmp(buffer,_PLSR_COMPASS1_ID,10)==0)
							{
								strcpy(PLSRBuffer,buffer);
								buffer[0] = '\0';
								_term_type=_TERM_PLSR;
							}
							else if(strncmp(buffer,_RMC_ID,5)==0)
							{
								strcpy(GPRMCBuffer,buffer);
								buffer[0] = '\0';
								_term_type=_TERM_RMC;
							}
							else // Message not of interest 
							{
							//	Serial.println("Message Not Interesting");
								_new_term=false;
								_term_type=_TERM_OTHER;
								bufferIndex = 0;
								buffer[0] = '\0';
							}
						}
							break;
					case _TERM_RMC:
						GPRMCBuffer[bufferIndex]=byteGPS;
						bufferIndex++;
						Serial.write(byteGPS);
						if(byteGPS == 10)
						{
							
							GPRMCBuffer[bufferIndex]='\0';
							extractData(GPRMCBuffer,GPRMC);
							bufferIndex=0;
							_new_term=false;
						}
						break;
					case _TERM_PLSR:
						PLSRBuffer[bufferIndex]=byteGPS;
						bufferIndex++;									
						if(byteGPS==10)
						{
							//Serial.write(PLSRBuffer);
							PLSRBuffer[bufferIndex]='\0';
							extractData(PLSRBuffer,PLSR);
							bufferIndex=0;
							_new_term=false;
						}
						break;
					}
				}
			}			
		}
		if(strlen(GPRMCBuffer)>0 && strlen(PLSRBuffer)>0)
		break;
	}
	return 1;
}


void RR_GPS::storeGPRMCData(char value[], int segment)
{
	switch(segment)
	{
	case TIME:
		strcpy(time,value);
		break;
	case STATUS:
		status = atoi(value);
		break;
	case LATITUDE:
		latitude = atof(value);
		break;
	case NS:
		if(value[0] == 'S')
			latitude *= -1;
		break;
	case LONGITUDE:
		longitude = atof(value);
		break;
	case EW:
		if(value[0] == 'W')
			longitude *= -1;
		break;
	case SPEED:
		spd = atof(value);
		break;
	case COURSE:
		course = atof(value);
		break;
	case DATE:
		strcpy( date,value);
		break;
	}
}

void RR_GPS::storePLSRData(char value[], int segment)
{
	switch(segment)
	{
	case DIR:
		dir = atof(value);
		break;
	case CSTATUS:
		cstatus = atoi(value);
		break;
	case FINT:
		fint = atof(value);
		break;
	case AX:
		ax = atof(value);
		break;
	case AY:
		ay = atof(value);
		break;
	case AZ:
		az = atof(value);
		break;
	case TEMPERATURE:
		temperature = atof(value);
		break;
	case MM:
		mm = atoi(value);
		break;
	case CCDS:
		ccds = atoi(value);
		break;
	}
}

void RR_GPS::extractData(char line[], int type)
{
	int index = 0, segment = 0, indexTemp = 0;
	char segmentData[15] = "";
	while(line[index]){
		if(line[index] == ','){
			segmentData[indexTemp] = '\0';
			if(type == GPRMC){
				storeGPRMCData(segmentData,segment);
			}else if(type == PLSR){
				storePLSRData(segmentData,segment);
			}
			segment++;
			indexTemp = 0;
		}else{
			segmentData[indexTemp] = line[index];
			indexTemp++;
			if(line[index] == '*'){
				// Checksum is being ignored here.
				return;
			}
		}
		index++;
	}
}

void RR_GPS::displayData(int type)
{
	if(type == GPRMC)
	{
		Serial.println(F(" Printing GPRMC Report"));
		Serial.print(F(" Time: "));
		Serial.println( time);
		Serial.print(F(" Status: "));
		Serial.println( status);
		Serial.print(F(" Latitude: "));
		Serial.println( latitude);
		Serial.print(F(" Longitude: "));
		Serial.println( longitude);
		Serial.print(F(" Speed: "));
		Serial.println( spd);
		Serial.print(F(" Course: "));
		Serial.println( course);
		Serial.print(F(" Date: "));
		Serial.println( date);
	}
	else if(type == PLSR)
	{
		Serial.println(F(" Printing PLSR Report"));
		Serial.print(F(" Direction: "));
		Serial.println( dir);
		Serial.print(F(" Calib Status: "));
		Serial.println( cstatus);
		Serial.print(F(" Field Intensity: "));
		Serial.println( fint);
		Serial.print(F(" Ax: "));
		Serial.println( ax);
		Serial.print(F(" Ay: "));
		Serial.println( ay);
		Serial.print(F(" Az: "));
		Serial.println( az);
		Serial.print(F(" Temp: "));
		Serial.println( temperature);
		Serial.print(F(" Mode: "));
		Serial.println( mm);
		Serial.print(F(" CCD Status: "));
		Serial.println( ccds);
	}   
}
