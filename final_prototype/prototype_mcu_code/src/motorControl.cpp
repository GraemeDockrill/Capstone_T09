
#include <motorControl.h>

long enc1_pos = 0;
long enc2_pos = 0;
long enc3_pos = 0;
long enc4_pos = 0;

int cyclic_stretching = 0;
int manual_control = 0;
int auto_control = 0;

int min_stretch_steps = 0;
int max_stretch_steps = 0;

int target_cycles = 0;
int current_cycles = 0;

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

    if(manual_control){
      // run steppers at speed
      stepper1.runSpeed();
      stepper2.runSpeed();
      stepper3.runSpeed();
      stepper4.runSpeed();
    }
    else if(auto_control){

      // run motors to target position
      stepper1.run();
      stepper2.run();
      stepper3.run();
      stepper4.run();
      
      if(cyclic_stretching){
        // if reached min stretch position
        if(stepper1.currentPosition() == min_stretch_steps && stepper2.currentPosition() == min_stretch_steps  && stepper3.currentPosition() == min_stretch_steps  && stepper4.currentPosition() == min_stretch_steps){
          setMotorTargets(max_stretch_steps);
          current_cycles++;
        }
        // if reached max stretch position
        else if(stepper1.currentPosition() == max_stretch_steps && stepper2.currentPosition() == max_stretch_steps  && stepper3.currentPosition() == max_stretch_steps  && stepper4.currentPosition() == max_stretch_steps){
          setMotorTargets(min_stretch_steps);
        }

        // if target cycles reached, stop
        if(current_cycles >= target_cycles){
          cyclic_stretching = 0;
          auto_control = 0;
        }
      }
      
    }

    // return mutex
    xSemaphoreGive(motor_mutex);

    // Sleep for 1 milliseconds.
    vTaskDelay((1L * configTICK_RATE_HZ) / 1000L);

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

void setManualMotorSpeeds(int speed){
  // take motor mutex
  xSemaphoreTake(motor_mutex, portMAX_DELAY);

  // set motor speeds to received bytes
  stepper1.setSpeed(speed);
  stepper2.setSpeed(speed);
  stepper3.setSpeed(speed);
  stepper4.setSpeed(speed);

  auto_control = 0;
  manual_control = 1;

  // return motor mutex
  xSemaphoreGive(motor_mutex);
}

void stopMotors(void){
  // take motor mutex
  xSemaphoreTake(motor_mutex, portMAX_DELAY);

  auto_control = 0;
  manual_control = 0;

  // return motor mutex
  xSemaphoreGive(motor_mutex);
}

void startCyclicStretching(int cycles){
  // take motor mutex
  xSemaphoreTake(motor_mutex, portMAX_DELAY);

  target_steps = min_stretch_steps;
  target_cycles = cycles;
  current_cycles = 0;

  manual_control = 0;
  auto_control = 1;

  // return motor mutex
  xSemaphoreGive(motor_mutex);
}

void setMotorTargets (int target){
  stepper1.moveTo(target);
  stepper2.moveTo(target);
  stepper3.moveTo(target);
  stepper4.moveTo(target);
}

void setMotorParameters(int start_steps, int speed){
  // take motor mutex
  xSemaphoreTake(motor_mutex, portMAX_DELAY);

  // set max stretch steps relative to current min stretch steps
  max_stretch_steps = min_stretch_steps - start_steps;

  // set motor speeds
  stepper1.setMaxSpeed(speed);
  stepper2.setMaxSpeed(speed);
  stepper3.setMaxSpeed(speed);
  stepper4.setMaxSpeed(speed);

  manual_control = 0;
  auto_control = 0;

  // return motor mutex
  xSemaphoreGive(motor_mutex);
}