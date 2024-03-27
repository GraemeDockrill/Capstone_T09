// motorControl.cpp header file for MECH45X cell stretcher capstone
// deals with the control of the 4 stepper motors of the device
// Written by Graeme Dockrill - 2024

#include <motorControl.h>

// encoder positions
long enc1_pos = 0;
long enc2_pos = 0;
long enc3_pos = 0;
long enc4_pos = 0;

int manual_control = 0;
int auto_control = 0;

int min_stretch_steps = 0;
int max_stretch_steps = 0;
int target_steps = 0;

int target_cycles = 0;
int current_cycles = 0;

int relative_target_pos_steps = 0;

// create trajectory
Trajectory_t g_trajectory;

// create trajectory axis parameters
Trajectory_Params_t g_trajectory_params;

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
      
    }
    else if(auto_control){

      // if motors reached target position
      if (g_trajectory.motor1.trajectory_finished && g_trajectory.motor2.trajectory_finished && g_trajectory.motor3.trajectory_finished && g_trajectory.motor4.trajectory_finished)
      {
        // if message waiting in the queue, then pop it
        if(xQueueReceive(motor_traj_queue, (Trajectory_Params_t*) &g_trajectory_params, 0)){

          // stop motors
          Motor_Control_Loop_Stop();

          // decrement the number of cycles for each and generate trajectories
          g_trajectory_params.cycles--;

          // generate trajectories
          Trajectory_Generate(&g_trajectory_params, &g_trajectory.motor1);
          Trajectory_Generate(&g_trajectory_params, &g_trajectory.motor2);
          Trajectory_Generate(&g_trajectory_params, &g_trajectory.motor3);
          Trajectory_Generate(&g_trajectory_params, &g_trajectory.motor4);
          
          // requeue if number of cycles > 0
          if(g_trajectory_params.cycles > 0){
            xQueueSend(motor_traj_queue, (Trajectory_Params_t*) &g_trajectory_params, 0);
          }

          // start motors
          Motor_Control_Loop_Start();
        }
      }      
    }

    // return mutex
    xSemaphoreGive(motor_mutex);

    // Sleep for 1 milliseconds.
    vTaskDelay((1L * configTICK_RATE_HZ) / 1000L);

  }
}

void Motor_Control_Initialize(void){

  // set up motor coordinate frames
  g_trajectory.motor1.motor_ID = 1;
  g_trajectory.motor2.motor_ID = 2;
  g_trajectory.motor3.motor_ID = 3;
  g_trajectory.motor4.motor_ID = 4;

  Motor_Hardware_Initialize();

  // assign QTIMER register to each motor
  Interrupt_Parameters_t motor1_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR1_ADDRESS,  // QTIMER 1
  };

  Interrupt_Parameters_t motor2_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR2_ADDRESS,  // QTIMER 2
  };

  Interrupt_Parameters_t motor3_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR3_ADDRESS,  // QTIMER 3
  };

  Interrupt_Parameters_t motor4_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR4_ADDRESS,  // QTIMER 4
  };

  // motor 1 QTIMER1
  Motor_Interrupt_Initialize(motor1_interrupt_params);      // fill registers for QTIMER
  attachInterruptVector(IRQ_QTIMER1, Motor1_QTIMER1_ISR);   // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER1);                             // enable interrupt vector
  // motor 2 QTIMER2
  Motor_Interrupt_Initialize(motor2_interrupt_params);
  attachInterruptVector(IRQ_QTIMER2, Motor2_QTIMER2_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER2);
  // motor 3 QTIMER3
  Motor_Interrupt_Initialize(motor3_interrupt_params);
  attachInterruptVector(IRQ_QTIMER3, Motor3_QTIMER3_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER3);
  // motor 4 QTIMER4
  Motor_Interrupt_Initialize(motor4_interrupt_params);
  attachInterruptVector(IRQ_QTIMER4, Motor4_QTIMER4_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER4);
}

void Motor_Interrupt_Initialize(Interrupt_Parameters_t interrupt_parameters){
  for(int i = 0; i < 1; i++){
    interrupt_parameters.timer_reg->CH[i].CTRL = 0; // stop TMR1 CH0
    interrupt_parameters.timer_reg->CH[i].SCTRL = 0x0;
    interrupt_parameters.timer_reg->CH[i].LOAD = 0x0; // initial value
    interrupt_parameters.timer_reg->CH[i].CTRL |= TMR_CTRL_PCS(0b1111); // clock prescaler of 128
    interrupt_parameters.timer_reg->CH[i].CTRL |= TMR_CTRL_LENGTH; // count until compare, then reload from CMPLD10
    interrupt_parameters.timer_reg->CH[i].CSCTRL |= TMR_CSCTRL_CL1(0b01);
    interrupt_parameters.timer_reg->CH[i].CSCTRL &= ~(TMR_CSCTRL_TCF1); // clear interrupt flag
    interrupt_parameters.timer_reg->CH[i].CSCTRL |= TMR_CSCTRL_TCF1EN; // enable COMP1 interrupt
    interrupt_parameters.timer_reg->CH[i].CNTR = 0; // reset counter value to 0
  }

  // for initial speedStep
  interrupt_parameters.timer_reg->CH[0].COMP1 = MIN_SPS_CMPLD; // count to this value
  interrupt_parameters.timer_reg->CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare

  // for acceleration
  interrupt_parameters.timer_reg->CH[1].COMP1 = ACCELERATION_INTERRUPT_INTERVAL; // count to this value
  interrupt_parameters.timer_reg->CH[1].CMPLD1 = ACCELERATION_INTERRUPT_INTERVAL; // start from this value after compare

  
}

void Motor_Control_Loop_Start(void){
  // start stepping speed at min speed
  IMXRT_TMR1.CH[0].COMP1 = MIN_SPS_CMPLD; // count to this value
  IMXRT_TMR1.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
  IMXRT_TMR2.CH[0].COMP1 = MIN_SPS_CMPLD; // count to this value
  IMXRT_TMR2.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
  IMXRT_TMR3.CH[0].COMP1 = MIN_SPS_CMPLD; // count to this value
  IMXRT_TMR3.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
  IMXRT_TMR4.CH[0].COMP1 = MIN_SPS_CMPLD; // count to this value
  IMXRT_TMR4.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare

  g_trajectory.motor1.current_steps_per_sec = MIN_SPS;
  g_trajectory.motor2.current_steps_per_sec = MIN_SPS;
  g_trajectory.motor3.current_steps_per_sec = MIN_SPS;
  g_trajectory.motor4.current_steps_per_sec = MIN_SPS;

  IMXRT_TMR1.CH[0].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR1.CH[1].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR2.CH[0].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR2.CH[1].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR3.CH[0].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR3.CH[1].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR4.CH[0].CTRL |= TMR_CTRL_CM(0b001);
  IMXRT_TMR4.CH[1].CTRL |= TMR_CTRL_CM(0b001);
}

void Motor_Control_Loop_Stop(void){
  // stop counting each QTIMER channel
  IMXRT_TMR1.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR1.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR2.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR2.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR3.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR3.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR4.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR4.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));

  // reset QTIMER channel counters to 0
  IMXRT_TMR1.CH[0].CNTR = 0;
  IMXRT_TMR1.CH[1].CNTR = 0;
  IMXRT_TMR2.CH[0].CNTR = 0;
  IMXRT_TMR2.CH[1].CNTR = 0;
  IMXRT_TMR3.CH[0].CNTR = 0;
  IMXRT_TMR3.CH[1].CNTR = 0;
  IMXRT_TMR4.CH[0].CNTR = 0;
  IMXRT_TMR4.CH[1].CNTR = 0;
}

void Trajectory_Generate(Trajectory_Params_t* trajectory_params, Motor_Control_t* motor_ptr){

  Trajectory_Motor_Params_t* trajectory_motor_params;


  /*
  
  ADD TING
  
  
  
  
  */


  // find out which axis has been passed
  if(motor_ptr->motor_ID == 1){
    trajectory_motor_params = &trajectory_params->motor1;
  }
  else if(motor_ptr->motor_ID == 2){
    trajectory_motor_params = &trajectory_params->motor2;
  }
  else if(motor_ptr->motor_ID == 3){
    trajectory_motor_params = &trajectory_params->motor3;
  }
  else{
    trajectory_motor_params = &trajectory_params->motor4;
  }

  // flag that the trajectory is not finished
  motor_ptr->trajectory_finished = false;

  // determine which direction the move is
  if(trajectory_motor_params->initial_pos_steps < trajectory_motor_params->target_pos_steps){
    motor_ptr->direction = POSITIVE_DIR;
  }
  else{
    motor_ptr->direction = NEGATIVE_DIR;
  }

  // solve for max speed with quadratic formula
  float delta_pos_steps = trajectory_motor_params->target_pos_steps - trajectory_motor_params->initial_pos_steps; // 8000
  float a = -1 / TRAJECTORY_ACCELERATION_SPSPS; // 0.0001
  float b = delta_pos_steps/trajectory_motor_params->avg_speed_sps; // 4
  float c = -delta_pos_steps; // 8000

  // calculate individual motor control parameters used in interrupts
  float max_speed_SPS = abs((-b + sqrtf(powf(b, 2) - 4*a*c))/(2*a)); // 
  motor_ptr->acc_pos_pulses = SPR_TO_PPR * powf(max_speed_SPS, 2)/(2*TRAJECTORY_ACCELERATION_SPSPS);
  motor_ptr->const_spd_pos_pulses = motor_ptr->acc_pos_pulses + (SPR_TO_PPR * ((delta_pos_steps * max_speed_SPS) / trajectory_motor_params->avg_speed_sps - 2* powf(max_speed_SPS, 2)/TRAJECTORY_ACCELERATION_SPSPS));
  motor_ptr->dec_pos_pulses = trajectory_motor_params->target_pos_steps * SPR_TO_PPR;
  motor_ptr->speed_increment = ((ACCELERATION_INTERRUPT_INTERVAL * TRAJECTORY_ACCELERATION_SPSPS) / QTIMER_FREQ_HZ) + 1; // constant slope of trapezoidal profile
  motor_ptr->current_steps_per_sec = MIN_SPS;
}

void Motor_Hardware_Initialize(void){
  // set motor PUL and DIR pins to outputs
  pinMode(MOT1_PUL, OUTPUT);
  pinMode(MOT1_DIR, OUTPUT);
  // pinMode(MOT2_PUL, OUTPUT);
  // pinMode(MOT2_DIR, OUTPUT);
  // pinMode(MOT3_PUL, OUTPUT);
  // pinMode(MOT3_DIR, OUTPUT);
  // pinMode(MOT4_PUL, OUTPUT);
  // pinMode(MOT4_DIR, OUTPUT);
}

void Motor1_QTIMER1_ISR(void){
  static int motor1_step_state = 0;
  motor1_step_state = !motor1_step_state;

  // read encoder1 position (used in stepSpeed & acceleration)
  int motor1_pos_pulse = encoder1.read();

  // if stepSpeed counter interrupt (TMR1_timer0)
  if(TMR1_CSCTRL0 & TMR_CSCTRL_TCF1){
    
    // if below the target
    if(motor1_pos_pulse < g_trajectory.motor1.dec_pos_pulses - ENCODER_TARGET_TOLERANCE){
      // move positive & take step
      digitalWrite(MOT1_DIR, POSITIVE_DIR);
      digitalWrite(MOT1_PUL, motor1_step_state);
    }
    // if above the target
    else if(motor1_pos_pulse > g_trajectory.motor1.dec_pos_pulses + ENCODER_TARGET_TOLERANCE){
      // move negative & take step
      digitalWrite(MOT1_DIR, NEGATIVE_DIR);
      digitalWrite(MOT1_PUL, motor1_step_state);
    }
    // if within tolerance of position
    else{
      Motor_Control_Loop_Stop();
      g_trajectory.motor1.trajectory_finished = true;
    }

    // clear stepping interrupt flag
    IMXRT_TMR1.CH[0].CSCTRL &= ~(TMR_CSCTRL_TCF1);
  }

  // if acceleration counter interrupt (TMR1_timer1)
  if(TMR1_CSCTRL1 & TMR_CSCTRL_TCF1){

    // if motor1 moving in +ve direction
    if(g_trajectory.motor1.direction == POSITIVE_DIR){

      // if in acceleration phase
      if(motor1_pos_pulse < g_trajectory.motor1.acc_pos_pulses){

        // if we are below the maximum motor speed, speed up
        if (QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec >= MAX_SPS_CMPLD){
          // speed up motor
          g_trajectory.motor1.current_steps_per_sec += g_trajectory.motor1.speed_increment;
          IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
        }
        // else stay at max
        else{
          IMXRT_TMR1.CH[0].CMPLD1 = MAX_SPS_CMPLD; // start from this value after compare
        }
      }

      // if in constant speed phase
      else if(motor1_pos_pulse < g_trajectory.motor1.const_spd_pos_pulses){
        // do nothing
      }

      // if in constant deceleration phase
      else if(motor1_pos_pulse < g_trajectory.motor1.dec_pos_pulses){

        // if we are above the minimum speed, slow down
        if(QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec <= MIN_SPS_CMPLD){
          // slow down motor
          g_trajectory.motor1.current_steps_per_sec -= g_trajectory.motor1.speed_increment;
          IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
        }
        // else stay at min
        else{
          IMXRT_TMR1.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
        }
      }
    }

    // if motor1 moving in -ve direction
    else if(g_trajectory.motor1.direction == NEGATIVE_DIR){

      // if in constant acceleration phase
      if(motor1_pos_pulse > g_trajectory.motor1.acc_pos_pulses){
        if (QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec >= MAX_SPS_CMPLD){
          // speed up motor
          g_trajectory.motor1.current_steps_per_sec += g_trajectory.motor1.speed_increment;
          IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
        }
        else{
          IMXRT_TMR1.CH[0].CMPLD1 = MAX_SPS_CMPLD; // start from this value after compare
        }
      }

      // if in constant speed phase
      else if(motor1_pos_pulse > g_trajectory.motor1.const_spd_pos_pulses){
        // do nothing
      }

      // if in constant deceleration phase
      else if(motor1_pos_pulse > g_trajectory.motor1.dec_pos_pulses){
        if(QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec <= MIN_SPS_CMPLD){
          // slow down motor
          g_trajectory.motor1.current_steps_per_sec -= g_trajectory.motor1.speed_increment;
          IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
        }
        else{
          IMXRT_TMR1.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
        }
      }
    }

    // disable acceleration interrupt flag
    IMXRT_TMR1.CH[1].CSCTRL &= ~(TMR_CSCTRL_TCF1);

  }

  // need for ARM CPU for interrupt to work
  __asm volatile ("dsb"); 

}

void Motor2_QTIMER2_ISR(void){
  // static int motor2_step_state = 0;
  // motor2_step_state = !motor2_step_state;

  // // read encoder1 position (used in stepSpeed & acceleration)
  // int motor2_pos = encoder2.read();

  // // if stepSpeed counter interrupt (TMR1_timer0)
  // if(TMR1_CSCTRL0 & TMR_CSCTRL_TCF1){
    
  //   // if below the target
  //   if(motor2_pos < g_trajectory.motor2.dec_pos_pulses - ENCODER_TARGET_TOLERANCE){
  //     // move positive & take step
  //     digitalWrite(MOT2_DIR, POSITIVE_DIR);
  //     digitalWrite(MOT2_PUL, motor2_step_state);
  //   }
  //   // if above the target
  //   else if(motor2_pos > g_trajectory.motor2.dec_pos_pulses + ENCODER_TARGET_TOLERANCE){
  //     // move negative & take step
  //     digitalWrite(MOT2_DIR, NEGATIVE_DIR);
  //     digitalWrite(MOT2_PUL, motor2_step_state);
  //   }
  //   // if within tolerance of position
  //   else{
  //     Motor_Control_Loop_Stop();
  //     g_trajectory.motor2.trajectory_finished = true;
  //   }

  //   // clear stepping interrupt flag
  //   IMXRT_TMR2.CH[0].CSCTRL &= ~(TMR_CSCTRL_TCF1);
  // }

  // // if acceleration counter interrupt (TMR1_timer1)
  // if(TMR2_CSCTRL1 & TMR_CSCTRL_TCF1){

  //   // if motor1 moving in +ve direction
  //   if(g_trajectory.motor2.direction == POSITIVE_DIR){

  //     // if in acceleration phase
  //     if(motor2_pos < g_trajectory.motor2.acc_pos_pulses){

  //       // if we are below the maximum motor speed, speed up
  //       if (QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec >= MAX_SPS_CMPLD){
  //         // speed up motor
  //         g_trajectory.motor2.current_steps_per_sec += g_trajectory.motor2.speed_increment;
  //         IMXRT_TMR2.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec;
  //       }
  //       // else stay at max
  //       else{
  //         IMXRT_TMR2.CH[0].CMPLD1 = MAX_SPS_CMPLD; // start from this value after compare
  //       }
  //     }

  //     // if in constant speed phase
  //     else if(motor2_pos < g_trajectory.motor2.const_spd_pos_pulses){
  //       // do nothing
  //     }

  //     // if in constant deceleration phase
  //     else if(motor2_pos < g_trajectory.motor2.dec_pos_pulses){

  //       // if we are above the minimum speed, slow down
  //       if(QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec <= MIN_SPS_CMPLD){
  //         // slow down motor
  //         g_trajectory.motor2.current_steps_per_sec -= g_trajectory.motor2.speed_increment;
  //         IMXRT_TMR2.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec;
  //       }
  //       // else stay at min
  //       else{
  //         IMXRT_TMR2.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
  //       }
  //     }
  //   }

  //   // if motor1 moving in -ve direction
  //   else if(g_trajectory.motor2.direction == NEGATIVE_DIR){

  //     // if in constant acceleration phase
  //     if(motor2_pos > g_trajectory.motor2.acc_pos_pulses){
  //       if (QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec >= MAX_SPS_CMPLD){
  //         // speed up motor
  //         g_trajectory.motor2.current_steps_per_sec += g_trajectory.motor2.speed_increment;
  //         IMXRT_TMR2.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec;
  //       }
  //       else{
  //         IMXRT_TMR2.CH[0].CMPLD1 = MAX_SPS_CMPLD; // start from this value after compare
  //       }
  //     }

  //     // if in constant speed phase
  //     else if(motor2_pos > g_trajectory.motor2.const_spd_pos_pulses){
  //       // do nothing
  //     }
      
  //     // if in constant deceleration phase
  //     else if(motor2_pos > g_trajectory.motor2.dec_pos_pulses){
  //       if(QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec <= MIN_SPS_CMPLD){
  //         // slow down motor
  //         g_trajectory.motor2.current_steps_per_sec -= g_trajectory.motor2.speed_increment;
  //         IMXRT_TMR2.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor2.current_steps_per_sec;
  //       }
  //       else{
  //         IMXRT_TMR2.CH[0].CMPLD1 = MIN_SPS_CMPLD; // start from this value after compare
  //       }
  //     }
  //   }

  //   // disable acceleration interrupt flag
  //   IMXRT_TMR2.CH[1].CSCTRL &= ~(TMR_CSCTRL_TCF1);

  // }




  TMR2_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR2_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);

  // need for ARM CPU for interrupt to work
  __asm volatile ("dsb");
}

void Motor3_QTIMER3_ISR(void){
  TMR3_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR3_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);

  // need for ARM CPU for interrupt to work
  __asm volatile ("dsb");
}

void Motor4_QTIMER4_ISR(void){
  TMR4_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR4_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);

  // need for ARM CPU for interrupt to work
  __asm volatile ("dsb");
}