// Firmware for MECH45X cell stretcher capstone
// Written by Graeme Dockrill - 2024

#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <math.h>
#include <defines.h>
#include <dataLogger.h>
#include <serialCommunicator.h>
#include <motorControl.h>

float x = 0;
float y3;
float y2;

void setup() {
  // put your setup code here, to run once:

  // g_trajectory.motor1 = {
  //   .axis = X_AXIS,
  //   .trajectory_finished = false,
  //   .direction = POSITIVE_DIR,
  //   .acc_pos = 0,
  //   .const_spd_pos = 0,
  //   .dec_pos = 10.0,
  //   .speed_increment = 0,
  //   .current_steps_per_sec = 200,
  // };

  portBASE_TYPE s1, s2, s3;

  // initialize circular buffer
  serial_buffer.createCircularBuffer(BUFFER_MAX_LENGTH);

  // initialize load cells
  //initializeLoadCells();

  // initialize motors
  Motor_Control_Initialize();
  
  
  Trajectory_Params_t trajectory_params = {
    .x = {
      .initial_pos_steps = 0.0,
      .target_pos_steps = 8000.0,
      .avg_speed_sps = 2000.0,
    },
    .y = {
      .initial_pos_steps = 0.0,
      .target_pos_steps = 8000.0,
      .avg_speed_sps = 2000.0,
    },
  };

  Trajectory_Generate(&trajectory_params, &g_trajectory.motor1);
  Trajectory_Generate(&trajectory_params, &g_trajectory.motor2);
  Trajectory_Generate(&trajectory_params, &g_trajectory.motor3);
  Trajectory_Generate(&trajectory_params, &g_trajectory.motor4);
  Motor_Control_Loop_Start();
  
  Serial.begin(9600);

  // create task at priority two
  s1 = xTaskCreate(LoggingDataReadThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(SerialThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // create task at priority two
  s3 = xTaskCreate(MotorControlThread, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);

  // check for creation errors
  if(motor_mutex==NULL || s1 != pdPASS || s2 != pdPASS || s3 != pdPASS){
    Serial.println("Creation problem");
    while(1);
  }

  //Serial.println("Starting the scheduler!");

  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}

void loop() {
  // nothing in here
}

// define function for comuptuing y from x
float computeY1 (float x){
  return sin(x);
}

float computeY2 (float x){
  return cos(x);
}

short computeEncoderTest(float x){
  return (short) rand()*x;
}
