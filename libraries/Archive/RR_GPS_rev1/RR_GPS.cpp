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

RR_GPS::RR_GPS(void)
#if USING_GPS_SOFTWARESERIAL
 :gps_SoftSerial(GPS_RX_PIN,GPS_TX_PIN)
#endif

{
	//_baud = baud;
	
	pinMode(GPS_ENABLE_PIN,OUTPUT);
	pinMode(GPS_VCC_PIN,OUTPUT);
	_new_term=false;
	
}

void RR_GPS::Enable()
{
	digitalWrite(GPS_VCC_PIN,HIGH);
	delay(20);
	digitalWrite(GPS_ENABLE_PIN,HIGH);
	Serial.println(GGA_OFF);
	delay(200);
	Serial.println(LLA_INIT);
	delay(200);
	Serial.println(GLL_OFF);
	delay(200);
	Serial.println(GSA_OFF);
	delay(200);
	Serial.println(GSV_OFF);
	delay(200);
	Serial.println(VTG_OFF);
	delay(200);
	Serial.println(PLSR_ON_1HZ);
	while(Serial.available())
	{
		if(Serial.read() == '$')
			break;
	}
	
}

void RR_GPS::Disable()
{
	digitalWrite(GPS_ENABLE_PIN,LOW);
}

int RR_GPS::readMessage(bool readRMC)
{
	char RMCBuffer[300] = "", PLSRBuffer[300] = "";
	while(1)
	{
		if(Serial.available())
		{        
			byteGPS = Serial.read();                  // Read a byte of the serial port
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
				else if(_new_term)                            // $ sign not carried to buffer
				{
					switch(_term_type)
					{
					case _TERM_OTHER:                         //New Term. Store enough characters to determine if of interest.
						buffer[bufferIndex] = byteGPS;        // Store valid Serial data in Buffer
						bufferIndex++;
						
						if(bufferIndex==10)
						{    
							if(strncmp(buffer,_PLSR_COMPASS1_ID,10)==0)
							{
								strcpy(PLSRBuffer,buffer);
								buffer[0] = '\0';
								_term_type=_TERM_PLSR;
							}
							else if(strncmp(buffer,_RMC_ID,5)==0)
							{
								if(readRMC){
									strcpy(RMCBuffer,buffer);
									buffer[0] = '\0';
									_term_type=_TERM_RMC;
								}
								else
								{
									_new_term=false;
									_term_type=_TERM_OTHER;
									bufferIndex = 0;
									buffer[0] = '\0';
								}
							}
							else // Message not of interest 
							{
							//    Serial.println("Message Not Interesting");
								_new_term=false;
								_term_type=_TERM_OTHER;
								bufferIndex = 0;
								buffer[0] = '\0';
							}
						}
						break;
					case _TERM_RMC:
						RMCBuffer[bufferIndex]=byteGPS;
						bufferIndex++;
					//	Serial.write(byteGPS);
						if(byteGPS == 10)
						{    
							RMCBuffer[bufferIndex]='\0';
							if(readRMC)
							{
								extractData(RMCBuffer,RMC);
							}
							bufferIndex=0;
							_new_term=false;
						}
						break;
					case _TERM_PLSR:
						PLSRBuffer[bufferIndex]=byteGPS;
						bufferIndex++;
						Serial.write(byteGPS);
						if(byteGPS==10)
						{
							Serial.write(PLSRBuffer);
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
		if(strlen(PLSRBuffer)>0)
		{
			if(!readRMC)
				{
					break;
				}
			else
				{
					if(strlen(RMCBuffer)>0)
						{
							break;
						}
				}
		}
	}
	return 1;
}

void RR_GPS::storeRMCData(char value[], int segment)
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
			
			if(type == RMC){
				
				storeRMCData(segmentData,segment);
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
	if(type == RMC)
	{
		Serial.println(F(" Printing GPRMC Report"));
		Serial.print(F(" Time: "));
		Serial.println(time);
		Serial.print(F(" Status: "));
		Serial.println(status);
		Serial.print(F(" Latitude: "));
		Serial.println(latitude);
		Serial.print(F(" Longitude: "));
		Serial.println(longitude);
		Serial.print(F(" Speed: "));
		Serial.println(spd);
		Serial.print(F(" Course: "));
		Serial.println(course);
		Serial.print(F(" Date: "));
		Serial.println(date);
	}
	else if(type == PLSR)
	{
		Serial.println(F(" Printing PLSR Report"));
		Serial.print(F(" Direction: "));
		Serial.println(dir);
		Serial.print(F(" Calib Status: "));
		Serial.println(cstatus);
		Serial.print(F(" Field Intensity: "));
		Serial.println(fint);
		Serial.print(F(" Ax: "));
		Serial.println(ax);
		Serial.print(F(" Ay: "));
		Serial.println(ay);
		Serial.print(F(" Az: "));
		Serial.println(az);
		Serial.print(F(" Temp: "));
		Serial.println(temperature);
		Serial.print(F(" Mode: "));
		Serial.println(mm);
		Serial.print(F(" CCD Status: "));
		Serial.println(ccds);
	}   
}

void RR_GPS::setTarget(float latitude, float longitude)
{
  target_latitude = latitude;
  target_longitude = longitude;
}

void RR_GPS::updateTargetDistance()
{
  // Using Haversine formula for calculating Over-the-surface distance
  // Source: http://www.movable-type.co.uk/scripts/latlong.html
  int R = 6371;   // Mean Earth radius
  float lat1 = toRad(latitude);
  float lat2 = toRad(target_latitude);
  float dlat = lat2-lat1, dlong = toRad(target_longitude)-toRad(longitude);
  float a = sin(dlat/2)*sin(dlat/2)+sin(dlong/2)*sin(dlong/2)*cos(lat1)*cos(lat2);
  float c = 2*atan2(sqrt(a),sqrt(1-a));
  distance_rem = R*c;
  // Using the formula for bearing to calculate the required current heading.
  float y = sin(dlong)*cos(lat2);
  float x = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlong);
  float bearing = atan2(y,x)*57.295779513; // Multiplied by constant to convert to decimal degrees.
  delta_heading_reqd = bearing - dir;
}

float RR_GPS::toRad(float value)
{
  int sec = (value - int(value))*100;
  int min = int(value)%100;
  int deg = int(value)/100;
  return deg*0.017453293 + min*0.000290888 + sec*0.000004848;
}
