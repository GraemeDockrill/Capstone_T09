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

// create trajectory
Trajectory_t g_trajectory;

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

      // run motors to target position

      
      // if(cyclic_stretching){
      //   // if reached min stretch position
      //   if(stepper1.currentPosition() == min_stretch_steps && stepper2.currentPosition() == min_stretch_steps  && stepper3.currentPosition() == min_stretch_steps  && stepper4.currentPosition() == min_stretch_steps){
      //     setMotorTargets(max_stretch_steps);
      //     current_cycles++;
      //   }
      //   // if reached max stretch position
      //   else if(stepper1.currentPosition() == max_stretch_steps && stepper2.currentPosition() == max_stretch_steps  && stepper3.currentPosition() == max_stretch_steps  && stepper4.currentPosition() == max_stretch_steps){
      //     setMotorTargets(min_stretch_steps);
      //   }

      //   // if target cycles reached, stop
      //   if(current_cycles >= target_cycles){
      //     cyclic_stretching = 0;
      //     auto_control = 0;
      //   }
      // }
      
    }

    // return mutex
    xSemaphoreGive(motor_mutex);

    // Sleep for 1 milliseconds.
    vTaskDelay((1L * configTICK_RATE_HZ) / 1000L);

  }
}

void Motor_Control_Initialize(void){

  Motor_Hardware_Initialize();

  Interrupt_Parameters_t motor1_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR1_ADDRESS,
  };

  Interrupt_Parameters_t motor2_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR2_ADDRESS,
  };

  Interrupt_Parameters_t motor3_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR3_ADDRESS,
  };

  Interrupt_Parameters_t motor4_interrupt_params = {
    .timer_reg = (IMXRT_TMR_t*)IMXRT_TMR4_ADDRESS,
  };

  Motor_Interrupt_Initialize(motor1_interrupt_params);
  attachInterruptVector(IRQ_QTIMER1, Motor1_QTIMER1_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER1);
  Motor_Interrupt_Initialize(motor2_interrupt_params);
  attachInterruptVector(IRQ_QTIMER2, Motor2_QTIMER2_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER2);
  Motor_Interrupt_Initialize(motor3_interrupt_params);
  attachInterruptVector(IRQ_QTIMER3, Motor3_QTIMER3_ISR);  // attach interrupt vector to ISR
  NVIC_ENABLE_IRQ(IRQ_QTIMER3);
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
  IMXRT_TMR1.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR1.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR2.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR2.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR3.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR3.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR4.CH[0].CTRL &= ~(TMR_CTRL_CM(0b001));
  IMXRT_TMR4.CH[1].CTRL &= ~(TMR_CTRL_CM(0b001));

  IMXRT_TMR1.CH[0].CNTR = 0;
  IMXRT_TMR1.CH[1].CNTR = 0;
  IMXRT_TMR2.CH[0].CNTR = 0;
  IMXRT_TMR2.CH[1].CNTR = 0;
  IMXRT_TMR3.CH[0].CNTR = 0;
  IMXRT_TMR3.CH[1].CNTR = 0;
  IMXRT_TMR4.CH[0].CNTR = 0;
  IMXRT_TMR4.CH[1].CNTR = 0;
}

void Motor_Hardware_Initialize(void){
  pinMode(MOT1_PUL, OUTPUT);
  pinMode(MOT1_DIR, OUTPUT);
}

void Motor1_QTIMER1_ISR(void){
  static int motor1_step_state = 0;
  motor1_step_state = !motor1_step_state;

  // read encoder1 position (used in stepSpeed & acceleration)
  int motor1_pos = encoder1.read();

  // if stepSpeed counter interrupt (TMR1_timer0)
  if(TMR1_CSCTRL0 & TMR_CSCTRL_TCF1){
    
    // if below the target
    if(motor1_pos < g_trajectory.motor1.dec_pos - ENCODER_TARGET_TOLERANCE){
      // move positive
      digitalWrite(MOT1_DIR, POSITIVE_DIR);
      // take step
      digitalWrite(MOT1_PUL, motor1_step_state);
    }
    // if above the target
    else if(motor1_pos > g_trajectory.motor1.dec_pos + ENCODER_TARGET_TOLERANCE){
      // move negative
      digitalWrite(MOT1_DIR, NEGATIVE_DIR);
      // take step
      digitalWrite(MOT1_PUL, motor1_step_state);
    }
    // within tolerance of position
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
    if(g_trajectory.motor1.direction == 1){

      // if in acceleration phase
      if(motor1_pos < g_trajectory.motor1.acc_pos){
        if (IMXRT_TMR1.CH[0].CMPLD1 >= MAX_SPS_CMPLD){
          // speed up motor
          g_trajectory.motor1.current_steps_per_sec += g_trajectory.motor1.slope;
          IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
        }
        else{
          IMXRT_TMR1.CH[0].CMPLD1 = MAX_SPS_CMPLD; // start from this value after compare
        }
      }
      // if in constant speed phase
      else if(motor1_pos < g_trajectory.motor1.const_spd_pos){
        // do nothing
      }
      // if in constant deceleration phase
      else if(motor1_pos < g_trajectory.motor1.dec_pos){
        // slow down motor
        g_trajectory.motor1.current_steps_per_sec -= g_trajectory.motor1.slope;
        IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
      }
    }
    // if motor1 moving in -ve direction
    else if(g_trajectory.motor1.direction == 0){

      // if in constant acceleration phase
      if(motor1_pos > g_trajectory.motor1.acc_pos){
        // speed up motor
        g_trajectory.motor1.current_steps_per_sec += g_trajectory.motor1.slope;
        IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
      }
      // if in constant speed phase
      else if(motor1_pos > g_trajectory.motor1.const_spd_pos){
        // do nothing
      }
      else if(motor1_pos > g_trajectory.motor1.dec_pos){
        // slow down motor
        g_trajectory.motor1.current_steps_per_sec -= g_trajectory.motor1.slope;
        IMXRT_TMR1.CH[0].CMPLD1 = QTIMER_FREQ_HZ / g_trajectory.motor1.current_steps_per_sec;
      }
    }

    // disable acceleration interrupt flag
    IMXRT_TMR1.CH[1].CSCTRL &= ~(TMR_CSCTRL_TCF1);

  }

  __asm volatile ("dsb"); 

}

void Motor2_QTIMER2_ISR(void){
  TMR2_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR2_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);
  __asm volatile ("dsb");
}

void Motor3_QTIMER3_ISR(void){
  TMR3_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR3_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);
  __asm volatile ("dsb");
}

void Motor4_QTIMER4_ISR(void){
  TMR4_CSCTRL0 &= ~(TMR_CSCTRL_TCF1);
  TMR4_CSCTRL1 &= ~(TMR_CSCTRL_TCF1);
  __asm volatile ("dsb");
}