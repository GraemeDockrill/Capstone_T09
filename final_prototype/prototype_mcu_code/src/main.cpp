#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <defines.h>
#include <math.h>

// create semaphore handle
SemaphoreHandle_t mutex, empty, full;
QueueHandle_t queue;

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

    // Serial.println("Thread 1: Waiting on Thread 2 to turn LED ON");

    // // Wait for signal from thread 2.
    // xSemaphoreTake(mutex, portMAX_DELAY);

    // Serial.println("Thread 1: Turning LED OFF");

    // // Turn LED off.
    // digitalWrite(LED_BUILTIN, LOW);

    //Serial.println("thread 1 computing y = x*x");

    y3 = computeY1(x);
    y2 = computeY2(x);

    //Serial.println("thread 1 taking empty buffer");
    // take empty buffer semaphore
    xSemaphoreTake(empty, portMAX_DELAY);

    //Serial.println("thread 1 taking mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);

    queue_message.time_stamp = (int) millis();
    queue_message.data1 = y3;
    queue_message.data2 = y2;
    queue_message.data3 = computeEncoderTest(x);
    queue_message.data4 = computeEncoderTest(x);
    queue_message.data5 = computeEncoderTest(x);
    queue_message.data6 = computeEncoderTest(x);

    //Serial.println("thread 1 enqueing");
    xQueueSend(queue, (void *) &queue_message, 0);

    //Serial.println("thread 1 releasing mutex");
    xSemaphoreGive(mutex);

    //Serial.println("thread 1 releasing give");
    // return new full buffer semaphore
    xSemaphoreGive(full);

    x += 0.1;

    // Sleep for 100 milliseconds.
    vTaskDelay((100L * configTICK_RATE_HZ) / 1000L);

  }
}

// declare thread function for thread 2
// takes any full buffer spots and sends data over UART
static void SerialThread(void* arg){
  
  pinMode(LED_BUILTIN, OUTPUT);

  while(1){

    //Serial.println("thread 2 taking full buffer");
    xSemaphoreTake(full, portMAX_DELAY);

    //Serial.println("thread 2 taking mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);

    //Serial.println("thread 2 dequeing");
    // remove message from queue
    xQueueReceive(queue, (void *) &COM_message.parsed_message, portMAX_DELAY);

    //Serial.println("thread 2 sending UART");
    // send message over UART
    Serial.write(255);
    for(int i = 0; i < 20; i++){
      Serial.write(COM_message.temp_byte[i]);         // write data bytes
    }
    Serial.write(ESCByte);

    //Serial.println("thread 2 releasing mutex");
    xSemaphoreGive(mutex);

    //Serial.println("thread 2 releasing empty");
    // signal a new queue spot has opened
    xSemaphoreGive(empty);

  }
}

void setup() {
  // put your setup code here, to run once:

  portBASE_TYPE s1, s2;

  // pinMode(LED_BUILTIN, OUTPUT);

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