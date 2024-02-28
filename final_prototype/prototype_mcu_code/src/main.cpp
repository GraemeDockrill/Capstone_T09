// Firmware for MECH45X cell stretcher capstone
// Written by Graeme Dockrill - 2024

#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <HX711_ADC.h>
#include <Encoder.h>
#include <defines.h>
#include <math.h>

// create semaphore handle
SemaphoreHandle_t mutex, empty, full;
QueueHandle_t queue;
CircularBuffer serial_buffer;

// create HX711 objects
HX711_ADC load_cell1(LOAD_CELL1_DATA_PIN, LOAD_CELL1_SCK_PIN);
HX711_ADC load_cell2(LOAD_CELL2_DATA_PIN, LOAD_CELL2_SCK_PIN);

// create encoder objects
Encoder encoder1(ENC1_A, ENC1_B);
Encoder encoder2(ENC2_A, ENC2_B);
Encoder encoder3(ENC3_A, ENC3_B);
Encoder encoder4(ENC4_A, ENC4_B);

// global variables
float x = 0;
float y3;
float y2;

// declare functions for computing y from given x value
float computeY1 (float x);
float computeY2 (float x);
short computeEncoderTest(float x);


// declare thread function for thread 1
// computes y = x*x every 100 ms and adds it to buffer
static void LoggingDataReadThread(void* arg){
  while(1){

    if(load_cell1.update() & load_cell2.update())
      load_cell_data_rdy = true;

    if(load_cell_data_rdy){

      // read load cells
      float load_cell1_data = load_cell1.getData();
      float load_cell2_data = load_cell2.getData();

      // read encoders
      short enc1_data = (short) encoder1.read();
      short enc2_data = (short) encoder2.read();
      short enc3_data = (short) encoder3.read();
      short enc4_data = (short) encoder4.read();

      // take empty buffer semaphore
      xSemaphoreTake(empty, portMAX_DELAY);

      // create and add message to queue
      queue_message.time_stamp = (int) millis();
      queue_message.data1 = load_cell1_data;
      queue_message.data2 = load_cell2_data;
      queue_message.data3 = enc1_data;
      queue_message.data4 = enc2_data;
      queue_message.data5 = enc3_data;
      queue_message.data6 = enc4_data;

      // enqueue message for serial thread
      xQueueSend(queue, (void *) &queue_message, 0);

      // return new full buffer semaphore
      xSemaphoreGive(full);

    }

    // y3 = computeY1(x);
    // y2 = computeY2(x);

    // // take empty buffer semaphore
    // xSemaphoreTake(empty, portMAX_DELAY);

    // // add data to message queue struct
    // queue_message.time_stamp = (int) millis();
    // queue_message.data1 = y3;
    // queue_message.data2 = y2;
    // queue_message.data3 = computeEncoderTest(x);
    // queue_message.data4 = computeEncoderTest(x);
    // queue_message.data5 = computeEncoderTest(x);
    // queue_message.data6 = computeEncoderTest(x);

    // // enqueue message for serial thread
    // xQueueSend(queue, (void *) &queue_message, 0);

    // // return new full buffer semaphore
    // xSemaphoreGive(full);

    // x += 0.1;

    // Sleep for 100 milliseconds.
    vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);

  }
}

// declare thread function for thread 2
// takes any full buffer spots and sends data over UART
static void SerialThread(void* arg){
  
  // pinMode(LED_BUILTIN, OUTPUT);

  while(1){
    // check if byte waiting in serial
    if(Serial.available() > 0){
      serial_buffer.enqueue(Serial.read());     // enqueue received byte
    }
    // if length of buffer is long enough for a message
    if(serial_buffer.queueCount() >= RECEIVE_MSG_SIZE){
      // dequeue received bytes
      start_byte = serial_buffer.dequeue();
      cmd_byte0 = serial_buffer.dequeue();
      data_byte0 = serial_buffer.dequeue();
      data_byte1 = serial_buffer.dequeue();
      data_byte2 = serial_buffer.dequeue();
      data_byte3 = serial_buffer.dequeue();
      esc_byte = serial_buffer.dequeue();

      // if switching back using esc_byte
      if(esc_byte & BIT3)
        data_byte0 = 255;
      if(esc_byte & BIT2)
        data_byte1 = 255;
      if(esc_byte & BIT1)
        data_byte2 = 255;
      if(esc_byte & BIT0)
        data_byte3 = 255;

      // combining bytes back together
      data_int0 = data_byte0 << 8;
      data_int0 = data_int0 + data_byte1;
      data_int1 = data_byte2 << 8;
      data_int1 + data_int1 + data_byte3;

      // if start byte is valid
      if(start_byte == 255){

        // parse data_int0 and data_int1 according to cmd_byte0

      }
    }
    // check if there is something in the send queue
    if(uxQueueMessagesWaiting(queue) > 0){

      // signal full spot is removed
      xSemaphoreTake(full, portMAX_DELAY);

      // remove message from queue
      xQueueReceive(queue, (void *) &COM_message.parsed_message, portMAX_DELAY);

      // send message over UART
      Serial.write(255);
      for(int i = 0; i < 20; i++){
        Serial.write(COM_message.temp_byte[i]);         // write data bytes
      }
      Serial.write(ESCByte);

      // signal a new queue spot has opened
      xSemaphoreGive(empty);

    }

  }
}

void setup() {
  // put your setup code here, to run once:

  portBASE_TYPE s1, s2;

  // initialize circular buffer
  serial_buffer.createCircularBuffer(BUFFER_MAX_LENGTH);

  // initialize load cells
  load_cell1.begin();
  load_cell2.begin();

  // startup, stabilization, and tare each module at the same time
  while(load_cell1_rdy + load_cell2_rdy < 2){
    if(!load_cell1_rdy) load_cell1_rdy = load_cell1.startMultiple(LOAD_CELL_STABILIZATION_TIME_MS, LOAD_CELL_TARE);
    if(!load_cell2_rdy) load_cell2_rdy = load_cell2.startMultiple(LOAD_CELL_STABILIZATION_TIME_MS, LOAD_CELL_TARE);
  }

  // calibrate load cells
  load_cell1.setCalFactor(LOAD_CELL1_CALIBRATION_FACTOR);
  load_cell2.setCalFactor(LOAD_CELL2_CALIBRATION_FACTOR);

  Serial.begin(9600);

  // create semaphores for thread safe queue
  mutex = xSemaphoreCreateCounting(1,1);
  full = xSemaphoreCreateCounting(QUEUE_MAX_LENGTH, 0);
  empty = xSemaphoreCreateCounting(QUEUE_MAX_LENGTH, QUEUE_MAX_LENGTH);

  queue = xQueueCreate(QUEUE_MAX_LENGTH, 20);

  // create task at priority two
  s1 = xTaskCreate(LoggingDataReadThread, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(SerialThread, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // check for creation errors
  if(mutex==NULL || s1 != pdPASS || s2 != pdPASS){
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