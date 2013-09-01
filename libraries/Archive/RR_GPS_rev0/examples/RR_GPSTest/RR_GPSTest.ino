
#include <SoftwareSerial.h>
#include <RR_CommonDefines.h>
#include <RR_GPS.h>

RR_GPS gps;

/***************************
Variables under GPS Class:
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
******************************/

void setup() 
{
	Serial.begin(HARDWARE_SERIAL_BAUD_RATE);  // This is used for both Serial Monitor and for reading GPS
	gps.Enable();      // This will clear the buffer and is mandatory after resetting the GPS
	Serial.write("Hi \n");
}

void loop() 
{
	Serial.println("Reading Message");
	gps.readMessage();            // This will read the GPS and collect the GPRMC and PLSR report and update the variables
	Serial.println("RMC Message");
	gps.displayData(RMC);   // This can be used to display all the data of the data from GPRMC report
	Serial.println("PLSR Message");
	gps.displayData(PLSR);    // This can be used to display all the data of the data from PLSR report
	Serial.print("direction: ");
	Serial.println(gps.dir);  // The individual variables can also be accessed for manipulation
}