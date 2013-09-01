//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
Common Macros for hardware pins, constants, coordinates, communication rates.
*/

#pragma once
#include <string.h>
#include <ctype.h>

#define TBUG Serial.println("Test");
//WINNING NUMBERS - MODIFY ACCORDINGLY
#define TARGET_LAT 3347.342f  //N  ddmm.mmmm
#define TARGET_LON - 8424.003f //W dddmm.mmmm
//#define CURRENT_LOCATION F("21.12345,W, 21.12345,N")
//STATE FLAGS
#define LAUNCHING 0
#define LAUNCHED 1
#define LANDED 2
#define NAVIGATING 3
#define FINISHED 4
//GENERAL
#define PI	3.14159f
#define HARDWARE_SERIAL_BAUD_RATE 9600
#define STAGE1_SAMPLING 100000 //.1sec
#define STAGE2_SAMPLING 10000 //.01sec
#define STAGE3_SAMPLING 100000 //.1sec
//GPS 
#define GPS_VCC_PIN 4
#define GPS_ENABLE_PIN 8
#define GPS_RX_PIN 9 //SoftwareSerial Only
#define GPS_TX_PIN 10 //SoftwareSerial Only
#define USING_GPS_SOFTWARESERIAL 0
//MOTOR 
#define MOTOR1_ENABLE_PIN 6
#define MOTOR1_LOGIC1_PIN 2
#define MOTOR1_LOGIC2_PIN 3
#define FORWARD 1
#define BACKWARD 0
//SERVO
#define SERVO_CONTROL_PIN 5
//PRESSURE SENSOR
#define PSENSOR_ENABLE_PIN 7
#define ACTUALSEAPRESSURE 101500
#define PRESSURERUNNINGAVGSIZE 10
#define LIFTOFFTHRESHOLD 25.0f
//ACCELEROMETER
#define ACC_RUNNINGAVGSIZE 5
#define ACC_X_ANALOGPIN 1
#define ACC_Y_ANALOGPIN 2
#define ACC_Z_ANALOGPIN 3
#define VOLTAGEREFERENCE 5
//SD CARD
#define OPENLOG_RX_PIN 11 //SoftwareSerial Only
#define OPENLOG_TX_PIN 12 //SoftwareSerial Only
#define OPENLOG_GRN_PIN 13 //Reset Pin



