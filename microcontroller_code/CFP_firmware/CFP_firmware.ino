//#include "DFRobot_HX711.h"
#include "HX711.h"

// defines
#define PUL_PIN 6
#define DIR_PIN 7
#define ENA_PIN 8
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2
#define calibration_factor -409000
#define setDelay 10

#define positive 0
#define negative 1
#define disabled 1
#define enabled 0

// declaring variables
int receivedChar;                     // character received from UART
bool newData;                         // if new data packet received
bool autoControl = false;             // if stepper being automatically controlled
bool manualControl = false;           // if manually controlling the stepper
bool stepState = false;               // energized state of PUL_PIN
bool dirState = negative;             // stepper direction 0 = positive, 1 = negative to DIR_PIN
bool cyclicTesting = false;           // if running cyclic tests
bool cyclicDirection = positive;      // direction of travel for cyclic testing

int currentPosSteps = 0;              // current stepper step position
int targetPosSteps = 0;               // target stepper step position
int membraneSizeSteps = 0;            // size of membrane in steps
int maxStrainSteps = 0;               // number of steps from 0 to reach max strain
int targetStrainCycles = 0;           // target number of strain cycles
int currentStrainCycles = 0;          // current number of strain cycles
int autoStepTime = 0;                 // received auto step time for OCR1A register
int manualStepTime = 0;               // received manual step time for OCR1A register
int startDelay;                       // small delay before moving the stage
float loadCellReading;                // reading from the loadcell

int dataInt;                          // combined dataByte0 and dataByte1

int loadCellCount;                    // counter for load cell data reading

// received data packet bytes
int startByte;
int cmdByte0;
int dataByte0;
int dataByte1;
int ESCByte;

// sending data packet bytes
int currentStepByte0 = 0;
int currentStepByte1 = 0;
int loadCellESCByte = 0;

// define bit constants
int BIT0 = 0x0001;
int BIT1 = 0x0002;
int BIT2 = 0x0004;
int BIT3 = 0x0008;
int BIT4 = 0x0016;
int BIT5 = 0x0032;
int BIT6 = 0x0064;
int BIT7 = 0x0128;

union u_tag {
   float temp_float ; 
   byte temp_byte[4] ;
} u;

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
Calculations (for 0.5ms): 
  System clock 16 Mhz and Prescalar 8;
  Timer 1 speed = 16Mhz/8 = 2 Mhz    
  Pulse time = 1/2 Mhz =  0.5us  
  Count up to = 0.5ms / 0.5us = 1000 (so this is the value the OCR register should have)
  This means 1 step every 10 ms
*/

// interrupt for timer2: time = 1/(16Mhz/1024) * 250 =  16ms;

// --------- PROGRAM ---------

void setup() {
  // put your setup code here, to run once:
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  digitalWrite(ENA_PIN, disabled);


  cli();  // interrupt disable

  cb = createCircularBuffer(BUFFER_SIZE); // create circular buffer at cb location

  // timer 1 (interrupt each 0.5ms)
  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= BIT1; // clock pre-scaler 8
  TIMSK1 |= BIT1; // compare select on OCR1A
  OCR1A = 1000;  // compare value for compare

  // timer 2 (interrupt each 1ms)
  TCCR2A = 0;
  TCCR2B = 0;

  TCCR2B |= BIT2 + BIT1 + BIT0; // clock prescaler 8
  TIMSK2 |= BIT2; // compare select on OCR2A
  OCR2B = 250;

  sei();  // interrupt enable

  // set up load cell sensor
  //DFRobot_HX711 MyScale(A2, A3);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.begin(38400);
  Serial.print("Arduino Ready!");

  // interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:

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
            targetPosSteps = maxStrainSteps;
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
            //startDelay = setDelay;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 9:   // return to zero position
            targetPosSteps = 0;
            autoControl = true;
            //startDelay = setDelay;
            digitalWrite(ENA_PIN, enabled);  // enable stepper
            break;
          case 10:  // cyclic stretching
            targetPosSteps = maxStrainSteps;
            currentStrainCycles = 0;
            autoControl = true;
            cyclicTesting = true;
            startDelay = setDelay;
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
    if(stepState == 1){
      if(dirState == positive)
        currentPosSteps++;
      else if(dirState == negative)
        currentPosSteps--;
    }
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
        startDelay = setDelay;
        cyclicDirection = negative; // switch direction of travel
      }
      // if negative cyclic limit reached
      else if(currentPosSteps <= 0 && cyclicDirection == negative){
        currentStrainCycles++;
        startDelay = setDelay;
        cyclicDirection = positive; // switch direction of travel
        targetPosSteps = maxStrainSteps;
      }

      // when target cycles reached, stop testing
      if(currentStrainCycles >= targetStrainCycles)
        cyclicTesting = false;
      
    }

    //startDelay--;
  }
  
  // update pins
  digitalWrite(PUL_PIN, stepState);
  digitalWrite(DIR_PIN, dirState);
}


// ISR for reading the data for load cell
ISR(TIMER2_COMPB_vect){
  
  TCNT2 = 0; // reset timer to 0
  loadCellCount++;
  if(loadCellCount>=3) {

    if(scale.is_ready())
      //u.temp_float = scale.read();
      u.temp_float = scale.get_units();
    // u.temp_float = 533174.1;

    currentStepByte0 = currentPosSteps >> 8;
    currentStepByte1 = currentPosSteps;

    if(currentStepByte0 >= 255)
      loadCellESCByte |= BIT5;
    if(currentStepByte1 >= 255)
      loadCellESCByte |= BIT4;
    if(u.temp_byte[0] >= 255){
      u.temp_byte[0] = 0;
      loadCellESCByte |= BIT3;
    }
    if(u.temp_byte[1] >= 255){
      u.temp_byte[1] = 0;
      loadCellESCByte |= BIT2;
    }
    if(u.temp_byte[2] >= 255){
      u.temp_byte[2] = 0;
      loadCellESCByte |= BIT1;
    }
    if(u.temp_byte[3] >= 255){
      u.temp_byte[3] = 0;
      loadCellESCByte |= BIT0;
    }

    Serial.write(255);  // startByte
    Serial.write(currentStepByte0);
    Serial.write(currentStepByte1);
    Serial.write(u.temp_byte[0]);
    Serial.write(u.temp_byte[1]);
    Serial.write(u.temp_byte[2]);
    Serial.write(u.temp_byte[3]);
    Serial.write(loadCellESCByte);

    loadCellESCByte = 0;

    loadCellCount = 0;
  }
}