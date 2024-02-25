#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>
#include <defines.h>
#include <math.h>

// create semaphore handle
SemaphoreHandle_t mutex, empty, full;
QueueHandle_t queue;

float x = 0;
float y;

// declare function for computing y from given x value
float computeY (float x);


// declare thread function for thread 1
// computes y = x*x every 100 ms and adds it to buffer
static void Thread1(void* arg){
  while(1){

    // Serial.println("Thread 1: Waiting on Thread 2 to turn LED ON");

    // // Wait for signal from thread 2.
    // xSemaphoreTake(mutex, portMAX_DELAY);

    // Serial.println("Thread 1: Turning LED OFF");

    // // Turn LED off.
    // digitalWrite(LED_BUILTIN, LOW);

    //Serial.println("thread 1 computing y = x*x");

    y =  computeY(x);

    //Serial.println("thread 1 taking empty buffer");
    // take empty buffer semaphore
    xSemaphoreTake(empty, portMAX_DELAY);

    //Serial.println("thread 1 taking mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);

    num.x = x;
    num.y = y;

    //Serial.println("thread 1 enqueing");
    xQueueSend(queue, (void *) &num, 0);

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
static void Thread2(void* arg){
  
  pinMode(LED_BUILTIN, OUTPUT);

  while(1){

    // // Turn LED on.
    // digitalWrite(LED_BUILTIN, HIGH);

    // Serial.println("Thread 2: Turining LED ON");

    // // Sleep for 200 milliseconds.
    // vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    // Serial.println("Thread 2: Asking Thread 1 to turn LED OFF");

    // // Signal thread 1 to turn LED off.
    // xSemaphoreGive(mutex);

    // // Sleep for 200 milliseconds.
    // vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    //Serial.println("thread 2 taking full buffer");
    xSemaphoreTake(full, portMAX_DELAY);

    //Serial.println("thread 2 taking mutex");
    xSemaphoreTake(mutex, portMAX_DELAY);

    //Serial.println("thread 2 dequeing");
    // remove message from queue
    xQueueReceive(queue, (void *) &message.temp_long, portMAX_DELAY);

    //Serial.println("thread 2 sending UART");
    // send message over UART
    Serial.write(255);
    Serial.write(message.temp_byte[0]);
    Serial.write(message.temp_byte[1]);
    Serial.write(message.temp_byte[2]);
    Serial.write(message.temp_byte[3]);
    Serial.write(message.temp_byte[4]);
    Serial.write(message.temp_byte[5]);
    Serial.write(message.temp_byte[6]);
    Serial.write(message.temp_byte[7]);
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

  queue = xQueueCreate(QUEUE_MAX_LENGTH, 8);

  // create task at priority two
  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

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
  // // put your main code here, to run repeatedly:
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);
  // x++;
  // y = x*x;
  // Serial.write(x);
  // Serial.write(y);
  // // Serial.write(",");
  // // Serial.write(y);
  // // Serial.write("\n");
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);
  // x++;
  // y = x*x;
  // Serial.write(x);
  // Serial.write(y);

}

// define function for comuptuing y from x
float computeY (float x){

  return sin(x);
}