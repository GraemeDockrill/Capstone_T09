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

#define QTIMER_FREQ_HZ 4687500.0
#define ACCELERATION_INTERRUPT_INTERVAL 65535.0

#define SPS_TO_PPR 1.25

#define MIN_SPS 1000
#define MAX_SPS 5000
#define MIN_SPS_CMPLD QTIMER_FREQ_HZ/MIN_SPS
#define MAX_SPS_CMPLD QTIMER_FREQ_HZ/MAX_SPS

#define TRAJECTORY_ACCELERATION_SPSPS 3000.0

#define ENCODER_TARGET_TOLERANCE 10

// #define SPEED_INCREMENT (ACCELERATION_INTERRUPT_INTERVAL / QTIMER_FREQ_HZ) * TRAJECTORY_ACCELERATION_SPSPS

#define X_AXIS 0
#define Y_AXIS 1

// declaring struct for motor parameters
typedef struct {
    IMXRT_TMR_t* timer_reg;
} Interrupt_Parameters_t;

typedef struct {
    bool axis;
    bool trajectory_finished;
    bool direction;
    float acc_pos;
    float const_spd_pos;
    float dec_pos;
    float speed_increment;
    float current_steps_per_sec;
} Motor_Control_t;

typedef struct {
    Motor_Control_t motor1;
    Motor_Control_t motor2;
    Motor_Control_t motor3;
    Motor_Control_t motor4;
} Trajectory_t;

typedef struct {
    int initial_pos_steps;
    int target_pos_steps;
    int avg_speed_sps; 
} Trajectory_Axis_Params_t;

typedef struct{
    Trajectory_Axis_Params_t x;
    Trajectory_Axis_Params_t y;
} Trajectory_Params_t;


extern Trajectory_t g_trajectory;

// declare thread for function for controlling the motors
// continually running control loop for the 4 motors
void MotorControlThread(void* arg);


void Motor_Control_Initialize(void);

void Motor_Interrupt_Initialize(Interrupt_Parameters_t interrupt_parameters);

void Motor_Control_Loop_Start(void);

void Motor_Control_Loop_Stop(void);

void Trajectory_Generate(Trajectory_Params_t* trajectory_params, Motor_Control_t* motor_ptr);

void Motor1_QTIMER1_ISR(void);

void Motor2_QTIMER2_ISR(void);

void Motor3_QTIMER3_ISR(void);

void Motor4_QTIMER4_ISR(void);

// initializes motor hardware pins
void Motor_Hardware_Initialize(void);

#endif