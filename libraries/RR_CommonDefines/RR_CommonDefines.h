//-------------------------------RESCUE ROBOT ---------------------------//
//----------------------------------------------------------------------//
/*
Common Macros for hardware pins, constants, coordinates, communication rates.
*/

#pragma once
#include <string.h>
#include <ctype.h>
#include <Arduino.h>

typedef enum {SLOW, MEDIUM, FAST} Speed_t;
typedef enum {LAUNCHING, ASCENDING, LANDING, LANDED, NAVIGATING, FINISHED} MainState_t;
typedef enum {CRUISING, OBSTACLED} NavigatingState_t;

#define RTYPE 1 //1 for Rover else for quadrotor
#define TBUG Serial.println("Test");
#define DBUG true
#define MOTOR_GEAR_RATIO 72
#define ENCODER_COUNT 48
#define WHEEL_RADIUS 0.07f
//WINNING NUMBERS - MODIFY ACCORDINGLY
#define TARGET_LAT 3347.342f  //N  ddmm.mmmm
#define TARGET_LON - 8424.003f //W dddmm.mmmm
//#define CURRENT_LOCATION F("21.12345,W, 21.12345,N")

//GENERAL
#define PI	3.14159f
#define HARDWARE_SERIAL_BAUD_RATE 9600
#define STAGE1_SAMPLING 100000 //.1sec
#define STAGE2_SAMPLING 10000 //.01sec
#define STAGE3_SAMPLING 100000 //.1sec

//GPS 
#define GPS_ENABLE_PIN 2

//SERIALS
#define GPS_Serial Serial1

//MACRO FUNCTIONS
#define TORAD(x) 
//MOTOR 
#define FORWARD 1
#define BACKWARD 0
#define M1DIR_PIN 23
#define M2DIR_PIN 21
#define MOTOR1_FB_PIN A0
#define MOTOR2_FB_PIN A1
#define MOTOR_SF_PIN 13
#define MOTOR_ENABLE_PIN 16
#define MOTOR_D_PIN 17
#define MOTOR1_PWM_PIN 22
#define MOTOR2_PWM_PIN 20

//DRIVER
#define THRESHOLD_ANGLE_LOWER 10
#define THRESHOLD_ANGLE_UPPER 60
#define SPEED_MAX 400
#define SPEED_CRUISE 350
#define SPEED_MANEUVER 300
#define SPEED_LOW 200

//ENCODER
#define ENCA1_PIN 3
#define ENCA2_PIN 4
#define ENCB1_PIN 5
#define ENCB2_PIN 6

//SD CARD
#define OPENLOG_GRN_PIN 11 //Reset Pin

//MOSFET
#define MOSFET_PIN 12

//RECEIVER
#define THRO_RX_PIN 3
#define AILE_RX_PIN 4
#define ELEV_RX_PIN 5
#define RUDD_RX_PIN 6



