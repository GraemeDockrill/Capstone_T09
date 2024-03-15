// motorControl.cpp header file for MECH45X cell stretcher capstone
// deals with the control of the 4 stepper motors of the device
// Written by Graeme Dockrill - 2024

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
int target_steps = 0;

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

// void motorInterruptInit(void){

  // /* TMR0_CTRL: CM=0,PCS=0,SCS=0,ONCE=0,LENGTH=1,DIR=0,COINIT=0,OUTMODE=0 */
  // TMR1_CTRL0 = TMR_CTRL_LENGTH; /* Stop all functions of the timer */
  // /* TMR0_SCTRL: TCF=0,TCFIE=0,TOF=0,TOFIE=0,IEF=0,IEFIE=0,IPS=0,INPUT=0,
  // Capture_Mode=0,MSTR=0,EEOF=0,VAL=0,FORCE=0,OPS=0,OEN=0 */
  // TMR1_SCTRL0 = 0x0;
  // TMR1_LOAD0 = 0x0; /* Reset load register */
  // TMR1_COMP10 = 46874; /* Set up compare 1 register */
  // TMR1_CMPLD10 = 46874; /* Also set the compare preload register */
  // /* TMR0_CSCTRL: DBG_EN=0,FAULT=0,ALT_LOAD=0,ROC=0,TCI=0,UP=0,OFLAG=0,TCF2EN=0,TCF1EN=1,
  // TCF2=0,TCF1=0,CL2=0,CL1=1 */
  // TMR1_CSCTRL0 = TMR_CSCTRL_TCF1EN + TMR_CSCTRL_CL1(0b01); /* Enable compare 1 interrupt and */
  // attachInterruptVector(IRQ_QTIMER4, motor1_QTIMER1_ISR);
  // /* compare 1 preload */
  // TMR1_CTRL0 &= TMR_CTRL_PCS(0b1111); /* Primary Count Source to IP_bus_clk / 128 */
  // TMR1_CNTR0 = 0x00; /* Reset counter register */
  // TMR1_CTRL0 &= TMR_CTRL_CM(0b001); /* Run counter */

// }

void motor::motorInterruptInit(void){

  // set up timer operation of channel 0 and 1
  for (int i = 0; i <= 1; i++){
    motorParams.timer_reg.CH[i].CTRL = 0; // stop TMR1 CH0
    motorParams.timer_reg.CH[i].SCTRL = 0x0;
    motorParams.timer_reg.CH[i].LOAD = 0x0; // initial value
    motorParams.timer_reg.CH[i].COMP1 = 0x0F00; // count to this value
    motorParams.timer_reg.CH[i].CMPLD1 = 0x0F00; // start from this value after compare
    motorParams.timer_reg.CH[i].CTRL |= TMR_CTRL_PCS(0b1111); // clock prescaler of 128
    motorParams.timer_reg.CH[i].CTRL |= TMR_CTRL_LENGTH; // count until compare, then reload from CMPLD10
    attachInterruptVector(IRQ_QTIMER1, motor1_QTIMER1_ISR);  // attach interrupt vector to ISR
    motorParams.timer_reg.CH[i].CSCTRL |= TMR_CSCTRL_CL1(0b01);
    motorParams.timer_reg.CH[i].CSCTRL &= ~(TMR_CSCTRL_TCF1); // clear interrupt flag
    motorParams.timer_reg.CH[i].CSCTRL |= TMR_CSCTRL_TCF1EN; // enable COMP1 interrupt
    motorParams.timer_reg.CH[i].CNTR = 0; // reset counter value to 0
    // motorParams.timer_reg.CH[i].CTRL &= TMR_CTRL_CM(0b001); // run counter    
  }

  // enable interrupt from timer peripheral
  NVIC_ENABLE_IRQ(motorParams.irq_vector);

}

void motor::motorEnable(bool enable){
  motorParams.timer_reg.CH[0].CNTR = 0; // reset counter value to 0
  motorParams.timer_reg.CH[1].CNTR = 0; // reset counter value to 0
  motorParams.timer_reg.CH[0].CTRL &= TMR_CTRL_CM(enable); // run counter
  motorParams.timer_reg.CH[1].CTRL &= TMR_CTRL_CM(enable); // run counter
}

// void motor1_QTIMER1_ISR(void){

//   // read encoder1 position (used in stepSpeed & acceleration)
//   motor1Pos = encoder1.read();

//   // if stepSpeed counter interrupt (TMR1_timer0)
//   if(TMR1_SCTRL0 & TMR_CSCTRL_TCF1){
    
//     // if below the target
//     if(motor1Pos < motor1Target){
//       // +step motor
//     }
//     // if above the target
//     else if(motor1Pos > motor1Target){
//       // -step motor
//     }
//     else{
//       // do nothing
//     }

//   }

//   // if acceleration counter interrupt (TMR1_timer1)
//   if(TMR1_SCTRL1 & TMR_CSCTRL_TCF1){
    
//     // if motor1 moving in +ve direction
//     if(motor1DIR == 1){

//       // if in acceleration phase
//       if(motor1Pos < motor1AccPos){
//         // speed up motor
//       }
//       // if in constant speed phase
//       else if(motor1Pos < motor1ConstSpdPos){
//         // do nothing
//       }
//       // if in constant deceleration phase
//       else if(motor1Pos < motor1DecPos){
//         // slow down motor
//       }
//       else{
//         // do nothing
//       }

//     }
//     // if motor1 moving in -ve direction
//     else if(motor1DIR == 0){

//       // if in constant acceleration phase
//       if(motor1Pos > motor1AccPos){
//         // speed up motor
//       }
//       // if in constant speed phase
//       else if(motor1Pos > motor1ConstSpdPos){
//         // do nothing
//       }
//       else if(motor1Pos > motor1DecPos){
//         // slow down motor
//       }
//       else{
//         // do nothing
//       }

//     }

//   }


// }

void motor::motor1_QTIMER1_ISR(void){

  // read encoder1 position (used in stepSpeed & acceleration)
  motor1Pos = *encoder_ptr.read();

  // if stepSpeed counter interrupt (TMR1_timer0)
  if(TMR1_SCTRL0 & TMR_CSCTRL_TCF1){
    
    // if below the target
    if(motor1Pos < motor1Target){
      // +step motor
    }
    // if above the target
    else if(motor1Pos > motor1Target){
      // -step motor
    }
    else{
      // do nothing
    }

    // clear stepping interrupt flag
    motorParams.timer_reg.CH[0].CSCTRL &= ~(TMR_CSCTRL_TCF1);

  }

  // if acceleration counter interrupt (TMR1_timer1)
  if(TMR1_SCTRL1 & TMR_CSCTRL_TCF1){
    
    // if motor1 moving in +ve direction
    if(motor1DIR == 1){

      // if in acceleration phase
      if(motor1Pos < motor1AccPos){
        // speed up motor
      }
      // if in constant speed phase
      else if(motor1Pos < motor1ConstSpdPos){
        // do nothing
      }
      // if in constant deceleration phase
      else if(motor1Pos < motor1DecPos){
        // slow down motor
      }
      else{
        // do nothing
      }

    }
    // if motor1 moving in -ve direction
    else if(motor1DIR == 0){

      // if in constant acceleration phase
      if(motor1Pos > motor1AccPos){
        // speed up motor
      }
      // if in constant speed phase
      else if(motor1Pos > motor1ConstSpdPos){
        // do nothing
      }
      else if(motor1Pos > motor1DecPos){
        // slow down motor
      }
      else{
        // do nothing
      }

    }

    // disable acceleration interrupt flag
    motorParams.timer_reg.CH[1].CSCTRL &= ~(TMR_CSCTRL_TCF1);

  }

  

  __asm volatile ("dsb"); 

}