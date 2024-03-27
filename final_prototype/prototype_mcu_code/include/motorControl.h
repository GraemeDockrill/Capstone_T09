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

#define SPR_TO_PPR 1.25
#define PPR_TO_SPR 0.8

// THESE NEED TO BE VERIFIED
#define MIN_SPS 1000
#define MAX_SPS 5000
#define MIN_SPS_CMPLD QTIMER_FREQ_HZ/MIN_SPS
#define MAX_SPS_CMPLD QTIMER_FREQ_HZ/MAX_SPS

#define TRAJECTORY_ACCELERATION_SPSPS 3000.0

#define ENCODER_TARGET_TOLERANCE 10

// #define SPEED_INCREMENT (ACCELERATION_INTERRUPT_INTERVAL / QTIMER_FREQ_HZ) * TRAJECTORY_ACCELERATION_SPSPS

#define X_AXIS 0
#define Y_AXIS 1

// declaring struct for interrupt register parameters
typedef struct {
    IMXRT_TMR_t* timer_reg;
} Interrupt_Parameters_t;

// struct for individual motor control parameters for a given trajectory
typedef struct {
    int motor_ID;
    bool trajectory_finished;
    bool direction;
    float acc_pos_pulses;
    float const_spd_pos_pulses;
    float dec_pos_pulses;
    float speed_increment;
    float current_steps_per_sec;
} Motor_Control_t;

// overall trajectory struct for the device (holds all motor control parameters)
typedef struct {
    Motor_Control_t motor1;
    Motor_Control_t motor2;
    Motor_Control_t motor3;
    Motor_Control_t motor4;
} Trajectory_t;

// parameters required for generating a trajectory with Trajectory_Generate
typedef struct {
    int initial_pos_steps;
    int target_pos_steps;
    int avg_speed_sps; 
} Trajectory_Motor_Params_t;

// struct holding trajectory parameters for each axis
typedef struct{
    Trajectory_Motor_Params_t motor1;
    Trajectory_Motor_Params_t motor2;
    Trajectory_Motor_Params_t motor3;
    Trajectory_Motor_Params_t motor4;
    int cycles;
} Trajectory_Params_t;

extern Trajectory_t g_trajectory;
extern Trajectory_Params_t g_trajectory_params;

extern int relative_target_pos_steps;

// declare thread for function for controlling the motors
// continually running control loop for the 4 motors
void MotorControlThread(void* arg);

// initializes motors to start the position control loop and trajectory
void Motor_Control_Initialize(void);

// sets up QTIMER interrupt registers for acceleration and stepspeed timers
// MOTOR1 QTIMER1 channel 0 - stepspeed | channel 1 - acceleration
// MOTOR2 QTIMER2 channel 0 - stepspeed | channel 1 - acceleration
// MOTOR3 QTIMER3 channel 0 - stepspeed | channel 1 - acceleration
// MOTOR4 QTIMER4 channel 0 - stepspeed | channel 1 - acceleration
void Motor_Interrupt_Initialize(Interrupt_Parameters_t interrupt_parameters);

// sets initial speed of stepper motors, sets initial trajectory speed, start counting QTIMER channels
void Motor_Control_Loop_Start(void);

// stop counting QTIMER channels and reset channel counters to 0
void Motor_Control_Loop_Stop(void);

// computes individual motor control parameters for a certain motor based
// on initial and target steps, as well as average speed for each axis
void Trajectory_Generate(Trajectory_Params_t* trajectory_params, Motor_Control_t* motor_ptr);

// ISR for control of motor1 uses QTIMER1
// channel 0 - stepspeed | channel 1 - acceleration
void Motor1_QTIMER1_ISR(void);

// ISR for control of motor2 uses QTIMER2
// channel 0 - stepspeed | channel 1 - acceleration
void Motor2_QTIMER2_ISR(void);

// ISR for control of motor3 uses QTIMER3
// channel 0 - stepspeed | channel 1 - acceleration
void Motor3_QTIMER3_ISR(void);

// ISR for control of motor4 uses QTIMER4
// channel 0 - stepspeed | channel 1 - acceleration
void Motor4_QTIMER4_ISR(void);

// initializes motor hardware pins as outputs
void Motor_Hardware_Initialize(void);

#endif