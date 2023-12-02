//#include "DFRobot_HX711.h"
#include "HX711.h"

// defines
#define PUL_PIN 6
#define DIR_PIN 7
#define ENA_PIN 8
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2
#define calibration_factor -260000

#define positive 0
#define negative 1
#define disabled 1
#define enabled 0

// declaring variables
int receivedChar;
bool newData;
bool autoControl = false;
bool manualControl = false;
bool stepState = false;
bool dirState = negative;             // 0 = positive, 1 = negative
bool cyclicTesting = false;
bool cyclicDirection = positive;

int ledState = 0;
int currentPosSteps = 0;
int targetPosSteps = 0;
int membraneSizeSteps = 0;
int maxStrainSteps = 0;
int maxStrainRate_steps_per_sec = 0;
int targetStrainCycles = 0;
int currentStrainCycles = 0;
int autoStepTime = 0;                 // received auto step time for OCR1A register
int manualStepTime = 0;               // received manual step time for OCR1A register
int dataInt;

int startByte;
int cmdByte0;
int dataByte0;
int dataByte1;
int ESCByte;

// define bit constants
int BIT0 = 0x0001;
int BIT1 = 0x0002;
int BIT2 = 0x0004;
int BIT3 = 0x0008;

HX711 scale;


// --------- CIRCULAR BUFFER STUFF ---------

// struct for circular buffer
typedef struct{

    unsigned int* buffer;
    int size;
    int front;
    int rear;
    int count;

} CircularBuffer;

// creates buffer in memory and returns pointer to it
CircularBuffer* createCircularBuffer(int size){
    CircularBuffer* cb = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    cb->buffer = (unsigned int*)malloc(size * sizeof(unsigned int));
    cb->size = size;
    cb->front = -1;
    cb->rear = -1;
    cb->count = 0;
    return cb;
}

// checks if buffer is full. returns 1 if full
int isFull(CircularBuffer* cb){
    return ((cb->rear + 1) % cb->size) == cb->front;
}

// checks if buffer is empty. returns 1 is empty
int isEmpty(CircularBuffer* cb){
    return cb->front == -1;
}

// enqueues a given int. sends an error over UART if queue full
void enqueue(CircularBuffer* cb, int data){
    if(isFull(cb)){
        // UART_string("Buffer is full, cannot enqueue!");
        return;
    }
    if(isEmpty(cb))
        cb->front = cb->rear = 0;
    else{
        cb->rear = (cb->rear + 1) % cb->size;
    }
    cb->buffer[cb->rear] = data;
    cb->count = cb->count + 1;
    return;
}

// dequeues a int from the buffer. returns the int if works, -1 otherwise
int dequeue(CircularBuffer* cb){
    if(isEmpty(cb)){
        // UART_string("Queue is empty, cannot dequeue!");
        return -1;
    }
    int data = cb->buffer[cb->front];
    if(cb->front == cb->rear)
        cb->front = cb->rear = -1;
    else
        cb->front = (cb->front + 1) % cb->size;

    cb->count = cb->count - 1;
    return data;
}

#define BUFFER_SIZE 50

CircularBuffer* cb;


/*
Calculations (for 500ms): 
  System clock 16 Mhz and Prescalar 8;
  Timer 1 speed = 16Mhz/8 = 2 Mhz    
  Pulse time = 1/2 Mhz =  0.5us  
  Count up to = 0.5ms / 0.5us = 1000 (so this is the value the OCR register should have)
  This means 1 step every 10 ms
*/


// --------- PROGRAM ---------

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  digitalWrite(ENA_PIN, disabled);


  cli();  // interrupt disable

  cb = createCircularBuffer(BUFFER_SIZE); // create circular buffer at cb location

  // timer 1 (interrupt each 500ms)
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= BIT1; // clock pre-scaler 8
  TIMSK1 |= BIT1; // compare select on OCR1A
  OCR1A = 1000;  // compare value for compare

  sei();  // interrupt enable

  // set up load cell sensor
  //DFRobot_HX711 MyScale(A2, A3);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.begin(9600);
  Serial.print("Arduino Ready!");

  // interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(LED_BUILTIN, ledState);

  // Serial.print("Reading: ");
  // Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  // Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  // Serial.println();

  checkSerialAvailable();
  newDataParsing();

}


void checkSerialAvailable(){

  if(Serial.available() > 0){
    receivedChar = Serial.read();
    enqueue(cb, receivedChar);
    newData = true;
  }  
}

void newDataParsing(){

  // if new packet received
  if(newData){

    // toggle LED
    if(receivedChar == 0)
      ledState = LOW;
    else if(receivedChar == 1)
      ledState = HIGH;

    if(cb->count >= 5){
      startByte = dequeue(cb);
      cmdByte0 = dequeue(cb);
      dataByte0 = dequeue(cb);
      dataByte1 = dequeue(cb);
      ESCByte = dequeue(cb);

      // Serial.write(startByte);
      // Serial.write(cmdByte0);
      // Serial.write(dataByte0);
      // Serial.write(dataByte1);
      // Serial.write(ESCByte);

      // switching back using ESCByte
      if(ESCByte & BIT1)
        dataByte0 = 255;
      if(ESCByte & BIT0)
        dataByte1 = 255;

      // combining dataBytes back together
      dataInt = dataByte0 << 8;
      dataInt = dataInt + dataByte1;

      // if startByte is valid
      if(startByte == 255){

        // disable automatic & manual control-reenable if cyclic loading chosen
        autoControl = false;
        manualControl = false;
        cyclicTesting = false;

        // parsing cmdByte0
        switch(cmdByte0){
          case 0:   // zero position
            currentPosSteps = 0;
            targetPosSteps = 0;
            break;
          case 1:   // move in +ve position
            manualControl = true;
            manualStepTime = dataInt;
            OCR1A = manualStepTime;
            dirState = positive;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 2:   // move in -ve position
            manualControl = true;
            manualStepTime = dataInt;
            OCR1A = manualStepTime;
            dirState = negative;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 3:   // set membrane size [steps]
            membraneSizeSteps = dataInt;
            break;
          case 4:   // set strain target [steps]
            maxStrainSteps = dataInt;
            break;
          case 5:   // set strain rate
            autoStepTime = dataInt;
            OCR1A = autoStepTime;
            break;
          case 6:   // set strain cycles
            targetStrainCycles = dataInt;
            break;
          case 7:   // set strain increment
            // Serial.print("cmdByte0: ");
            // Serial.println(cmdByte0);
            break;
          case 8:   // stretch to max strain
            targetPosSteps = maxStrainSteps;
            autoControl = true;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 9:   // return to zero position
            targetPosSteps = 0;
            autoControl = true;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 10:  // cyclic stretching
            targetPosSteps = maxStrainSteps;
            currentStrainCycles = 0;
            autoControl = true;
            cyclicTesting = true;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 11:  // STOP STEPPER
            digitalWrite(ENA_PIN, disabled);  // disable stepper
            break;
          default:
            break;
        }

      }
    }

    // finished reading packet
    newData = false;
  }
}


// ISR for stepping the stepper motor
ISR(TIMER1_COMPA_vect){
  TCNT1 = 0; // reset timer to 0

  // control type
  if(manualControl){
    stepState = !stepState; // switch state every interrupt
  }
  else if(autoControl){
  
    // check what side of target we're on and switch stepState
    if(currentPosSteps > targetPosSteps){  // if above target, move back
      dirState = negative;
      stepState = !stepState; // switch state every interrupt
      if(stepState == 1)
        currentPosSteps--;
    }
    else if(currentPosSteps < targetPosSteps){ // if below target move forward
      dirState = positive;
      stepState = !stepState; // switch state every interrupt
      if(stepState == 1)
        currentPosSteps++;
    }

    // if cyclic stretching, move accordingly
    if(cyclicTesting){

      // if positive cyclic limit reached
      if(currentPosSteps >= targetPosSteps && cyclicDirection == positive){
        targetPosSteps = 0;
        cyclicDirection = negative; // switch direction of travel
        Serial.println("1");
      }
      // if negative cyclic limit reached
      else if(currentPosSteps <= 0 && cyclicDirection == negative){
        currentStrainCycles++;
        cyclicDirection = positive; // switch direction of travel
        targetPosSteps = maxStrainSteps;
        Serial.println("0");
      }

      // when target cycles reached, stop testing
      if(currentStrainCycles >= targetStrainCycles)
        cyclicTesting = false;
    }
  }
  
  // update pins
  digitalWrite(PUL_PIN, stepState);
  digitalWrite(DIR_PIN, dirState);
}