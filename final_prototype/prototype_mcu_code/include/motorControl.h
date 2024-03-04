

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

// create motor objects
extern AccelStepper stepper1;
extern AccelStepper stepper2;
extern AccelStepper stepper3;
extern AccelStepper stepper4;

// create encoder objects
extern Encoder encoder1;
extern Encoder encoder2;
extern Encoder encoder3;
extern Encoder encoder4;

// declare thread for function for controlling the motors
// continually running control loop for the 4 motors
void MotorControlThread(void* arg);

// initialize motors with parameters
void initializeMotors(void);

// set motor speeds for manual movement
void setManualMotorSpeeds(int speed);

// stop motors
void stopMotors(void);

// start cyclic stretching test
void startCyclicStretching(int cycles);

// sets all motor targets
void setMotorTargets (int target);

// sets all motor parameters
void setMotorParameters(int start_steps, int speed);

#endif