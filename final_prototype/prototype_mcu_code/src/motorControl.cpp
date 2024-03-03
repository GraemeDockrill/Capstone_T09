
#include <motorControl.h>

long enc1_pos = 0;
long enc2_pos = 0;
long enc3_pos = 0;
long enc4_pos = 0;

int cyclic_stretching = 0;

AccelStepper stepper1(AccelStepper::DRIVER, MOT1_PUL, MOT1_DIR);
AccelStepper stepper2(AccelStepper::DRIVER, MOT2_PUL, MOT2_DIR);
AccelStepper stepper3(AccelStepper::DRIVER, MOT3_PUL, MOT3_DIR);
AccelStepper stepper4(AccelStepper::DRIVER, MOT4_PUL, MOT4_DIR);

// create encoder objects
Encoder encoder1(ENC1_A, ENC1_B);
Encoder encoder2(ENC2_A, ENC2_B);
Encoder encoder3(ENC3_A, ENC3_B);
Encoder encoder4(ENC4_A, ENC4_B);

void MotorControlThread(void* arg){
  while(1){
    // take mutex for updating motor target
    xSemaphoreTake(motor_mutex, portMAX_DELAY);

    // check new move command

    // return mutex
    xSemaphoreGive(motor_mutex);

    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();

    // Sleep for 10 milliseconds.
    vTaskDelay((10L * configTICK_RATE_HZ) / 1000L);

  }
}

void initializeMotors(void){
    // set motor parameters
    stepper1.setAcceleration(MOTOR_MAX_ACCELERATION);
    stepper1.setMaxSpeed(MOTOR_MAX_SPEED);
    stepper2.setAcceleration(MOTOR_MAX_ACCELERATION);
    stepper2.setMaxSpeed(MOTOR_MAX_SPEED);
    stepper3.setAcceleration(MOTOR_MAX_ACCELERATION);
    stepper3.setMaxSpeed(MOTOR_MAX_SPEED);
    stepper4.setAcceleration(MOTOR_MAX_ACCELERATION);
    stepper4.setMaxSpeed(MOTOR_MAX_SPEED);
}