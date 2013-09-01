/******************************************************************
* RR_GPS.h - Libary for using the GPS module - LS20126 from Sparkfun *
******************************************************************/

#include <SoftwareSerial.h>
#include <../RR_CommonDefines/RR_CommonDefines.h>

#ifndef RR_GPS_h
  #if defined(ARDUINO) && ARDUINO >= 100
	// Choose Arduino.h for IDE 1.0
	#include "Arduino.h"
  #else
	// Choose WProgram.h if IDE is older than 1.0
	#include "WProgram.h"
#endif

  #define RR_GPS_h
  
  #include <string.h>
  #include <stdlib.h>

  #define GPRMC 1
  #define PLSR 2

	// NMEA MESSAGES
	// TO TURN MESSAGES OFF
	#define GGA_OFF F("$PSRF103,00,00,00,01*24")
	#define GLL_OFF F("$PSRF103,01,00,00,01*25")
	#define GSA_OFF F("$PSRF103,02,00,00,01*26")
	#define GSV_OFF F("$PSRF103,03,00,00,01*27")
	#define RMC_OFF F("$PSRF103,04,00,00,01*20")
	#define VTG_OFF F("$PSRF103,05,00,00,01*21")

	//TO TURN ON WITH FREQUENCY OF 1HZ
	#define GGA_ON_1HZ F("$PSRF103,00,00,01,01*25")
	#define GLL_ON_1HZ F("$PSRF103,01,00,01,01*24")
	#define GSA_ON_1HZ F("$PSRF103,02,00,01,01*27")
	#define GSV_ON_1HZ F("$PSRF103,03,00,01,01*26")
	#define RMC_ON_1HZ F("$PSRF103,04,00,01,01*21")
	#define VTG_ON_1HZ F("$PSRF103,05,00,01,01*20")

	//CHANGE PLSR FREQUENCY TO 5HZ
	#define PLSR_ON_5HZ F("$PLSC,248,1,2*1D")
	
	//CHANGE PLSR FREQUENCY TO 1HZ
	#define PLSR_ON_1HZ F("$PLSC,248,0*0F")

  #define _PLSR_SPEED_ID	"PLSR,245,7"
  #define _PLSR_COMPASS1_ID	"PLSR,245,1" 
  #define _GGA_ID	"GPGGA"
  #define _RMC_ID	"GPRMC"

  #define MID 0        // Message ID
  #define TIME 1       // UTC Time
  #define STATUS 2     // Data Status
  #define LATITUDE 3   // Latitude
  #define NS 4         // North or South
  #define LONGITUDE 5  // Longitude
  #define EW 6         // East or West
  #define SPEED 7      // Speed
  #define COURSE 8     // Course
  #define DATE 9       // Date
  #define MVAR 10      // Magnetic Variation
  #define MODE 11      // Mode
  
  #define SID 0         // Sentence ID
  #define DIR 3         // Direction
  #define CSTATUS 4     // Calibration Status
  #define FINT 5        // Field Intensity
  #define AX 6          // Acceleration X
  #define AY 7          // Acceleration Y
  #define AZ 8          // Acceleration Z
  #define TEMPERATURE 9 // Temperature
  #define MM 10          // Mounting Mode
  #define CCDS 11        // Current Calibration Data Status

  class RR_GPS{
	public:
		
	  RR_GPS(void);


	  void Enable();
	  void Disable();
	  int readMessage();
	  void displayData(int type);
	  
	  
	  //TODO: REQUIRED NEW METHODS

	 // void getTargetDistance( //Return Target heading and 







	  //


	  // GPRMC Data
	  char time[13];   // UTC hh:mm:ss.sss
	  char status;     // A = Valid data and V = Warning
	  float latitude;  // Positive values => North; Negative value => South
	  float longitude; // Positive values => East; Negative value => West
	  float spd;     // Speed over ground in Knots
	  float course;    // Course over ground in degrees
	  char date[9];    // DD/MM/YY
	  float mvar;      // Magnetic variation in degrees
	  char mode;       // A = Automatic; D = DGPS; E = DR

	  // PLSR Data
	  float dir;       // Magnetic Direction (0 to 360 deg), 0 => North
	  int cstatus;     // Auto-calibration status, 7 => Complete
	  int fint;        // Magnetic Field intensity (0 to 1000)
	  int ax;          // Acceleration X (-512 to 511) +/- 2G
	  int ay;          // Acceleration Y (-512 to 511) +/- 2G
	  int az;          // Acceleration Z (-512 to 511) +/- 2G
	  int temperature; // Module Temperature in Celsius
	  int mm;          // Mounting Mode (0 to 7) default: 0
	  int ccds;        // Current calibrations data status: non-zero => valid, 0 => invalid
	
	private:

	  enum {_TERM_OTHER=0, _TERM_RMC=1, _TERM_PLSR=2 };
	  int _baud;
	  int _enpin;
	  byte _term_type;
	  void extractData(char line[], int type);
	  void storePLSRData(char value[], int segment);
	  void storeGPRMCData(char value[], int segment);
	  bool _new_term;

	  SoftwareSerial gps_SoftSerial;
  };
#endif


