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

#define ACCELERATION_INTERRUPT_INTERVAL 65535
#define QTIMER_FREQ_HZ 4687500

#define MIN_SPS 200
#define MAX_SPS 2000
#define MIN_SPS_CMPLD QTIMER_FREQ_HZ/MIN_SPS
#define MAX_SPS_CMPLD QTIMER_FREQ_HZ/MAX_SPS

// declaring struct for motor parameters
typedef struct {
    IMXRT_TMR_t* timer_reg;
} Interrupt_Parameters_t;

typedef struct {
    bool trajectory_finished;
    bool direction;
    int acc_pos;
    int const_spd_pos;
    int dec_pos;
    int slope;
    int current_steps_per_sec;
} Motor_Control_t;

typedef struct {
    Motor_Control_t motor1;
    Motor_Control_t motor2;
    Motor_Control_t motor3;
    Motor_Control_t motor4;
} Trajectory_t;

extern Trajectory_t g_trajectory;

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