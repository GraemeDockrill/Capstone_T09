#include <Arduino.h>
#include <FreeRTOS_TEENSY4.h>

// create semaphore handle
SemaphoreHandle_t sem;

int x = 0;
int y;

// declare thread function for thread 1
static void Thread1(void* arg){
  while(1){

    Serial.println("Thread 1: Waiting on Thread 2 to turn LED ON");

    // Wait for signal from thread 2.
    xSemaphoreTake(sem, portMAX_DELAY);

    Serial.println("Thread 1: Turning LED OFF");

    // Turn LED off.
    digitalWrite(LED_BUILTIN, LOW);
  }
}

// declare thread function for thread 2
static void Thread2(void* arg){
  
  pinMode(LED_BUILTIN, OUTPUT);

  while(1){

    // Turn LED on.
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("Thread 2: Turining LED ON");

    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);

    Serial.println("Thread 2: Asking Thread 1 to turn LED OFF");

    // Signal thread 1 to turn LED off.
    xSemaphoreGive(sem);

    // Sleep for 200 milliseconds.
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}

void setup() {
  // put your setup code here, to run once:

  portBASE_TYPE s1, s2;

  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  sem = xSemaphoreCreateCounting(1,0);

  // create task at priority two
  s1 = xTaskCreate(Thread1, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  // create task at priority one
  s2 = xTaskCreate(Thread2, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  // check for creation errors
  if(sem==NULL || s1 != pdPASS || s2 != pdPASS){
    Serial.println("Creation problem");
    while(1);
  }

  Serial.println("Starting the scheduler!");

  // start scheduler
  vTaskStartScheduler();
  Serial.println("Insufficient RAM");
  while(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  x++;
  y = x*x;
  Serial.write(x);
  Serial.write(y);
  // Serial.write(",");
  // Serial.write(y);
  // Serial.write("\n");
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  x++;
  y = x*x;
  Serial.write(x);
  Serial.write(y);

}