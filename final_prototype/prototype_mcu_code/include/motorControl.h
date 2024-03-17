// motorControl.h header file for MECH45X cell stretcher capstone
// Written by Graeme Dockrill - 2024

#ifndef motorControl_h
#define motorControl_h

#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <Encoder.h>
#include <AccelStepper.h>
#include <math.h>
#include <defines.h>
#include <dataLogger.h>
#include <serialCommunicator.h>

// create encoder objects
extern Encoder encoder1;
extern Encoder encoder2;
extern Encoder encoder3;
extern Encoder encoder4;

// declaring struct for motor parameters
typedef struct {
   IMXRT_TMR_t* timer_reg;
} Interrupt_Parameters_t;

// declare thread for function for controlling the motors
// continually running control loop for the 4 motors
void MotorControlThread(void* arg);


void Motor_Control_Initialize(void);

void Motor_Interrupt_Initialize(Interrupt_Parameters_t interrupt_parameters);

void Motor_Control_Loop_Start(void);

void Motor_Control_Loop_Stop(void);

void Motor1_QTIMER1_ISR(void);

void Motor2_QTIMER2_ISR(void);

void Motor3_QTIMER3_ISR(void);

void Motor4_QTIMER4_ISR(void);

// initializes motor hardware pins
void Motor_Hardware_Initialize(void);

#endif