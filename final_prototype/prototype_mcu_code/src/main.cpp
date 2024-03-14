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

  PIT_TFLG0 = 1;

  portBASE_TYPE s1, s2, s3;

  // initialize circular buffer
  serial_buffer.createCircularBuffer(BUFFER_MAX_LENGTH);

  // initialize load cells
  //initializeLoadCells();

  // initialize motors
  //initializeMotors();
  
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
