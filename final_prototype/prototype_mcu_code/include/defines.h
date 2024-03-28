// defines.h header file for MECH45X cell stretcher capstone
// declares variables that will be used across the program
// Written by Graeme Dockrill - 2024

#ifndef defines_h
#define defines_h

// header file for hardware register defines
#include "imxrt.h"

// load cell pins
#define LOAD_CELL1_SCK_PIN 27
#define LOAD_CELL2_SCK_PIN 13
#define LOAD_CELL1_DATA_PIN 28
#define LOAD_CELL2_DATA_PIN 41

// encoder pins
#define ENC1_A 8
#define ENC1_B 9
#define ENC2_A 6
#define ENC2_B 7
#define ENC3_A 4
#define ENC3_B 5
#define ENC4_A 2
#define ENC4_B 3

// motor driver pins
#define MOT1_PUL 17
#define MOT1_DIR 16
#define MOT2_PUL 19
#define MOT2_DIR 18
#define MOT3_PUL 21
#define MOT3_DIR 20
#define MOT4_PUL 23
#define MOT4_DIR 22

// motor direction
#define POSITIVE_DIR 1
#define NEGATIVE_DIR 0

// define bit constants
#define BIT0 0x0001
#define BIT1 0x0002
#define BIT2 0x0004
#define BIT3 0x0008
#define BIT4 0x0016
#define BIT5 0x0032
#define BIT6 0x0064
#define BIT7 0x0128

// declare received UART message bytes
extern int start_byte;
extern int cmd_byte0;
extern int data_byte0;
extern int data_byte1;
extern int data_byte2;
extern int data_byte3;
extern int esc_byte;

// declare combined UART data bytes
extern int data_int0;
extern int data_int1;

extern int new_data;
extern int received_char;

extern int ESCByte;        // for sending over UART

#define QUEUE_MAX_LENGTH 100
#define BUFFER_MAX_LENGTH 100
#define RECEIVE_MSG_SIZE 7
#define LOAD_CELL_STABILIZATION_TIME_MS 2000
#define LOAD_CELL_TARE true
#define LOAD_CELL1_CALIBRATION_FACTOR 1000
#define LOAD_CELL2_CALIBRATION_FACTOR 1000

#define MOTOR_MAX_SPEED 200     // steps per second
#define MOTOR_MAX_ACCELERATION 100      // steps per second per second
#define MOTOR_PPR 800
#define ENCODER_PPR 1000
#define LEAD_SCREW_PITCH 5      // mm / rev

extern int load_cell1_rdy;
extern int load_cell2_rdy;
extern int load_cell_data_rdy;
extern int cyclic_stretching;
extern int manual_control;
extern int auto_control;

extern int target_cycles;
extern int current_cycles;

extern int target_steps;
extern int min_stretch_steps;
extern int max_stretch_steps;

extern long enc1_pos;
extern long enc2_pos;
extern long enc3_pos;
extern long enc4_pos;

// global variables
extern float x;
extern float y3;
extern float y2;

// declare functions for computing y from given x value
float computeY1 (float x);
float computeY2 (float x);
short computeEncoderTest(float x);

#endif