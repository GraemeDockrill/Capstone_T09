//#include "DFRobot_HX711.h"
#include "HX711.h"

// definiting variables
int receivedChar;
bool newData;
bool autoControl = false;
bool manualControl = false;
int ledState = 0;
int currentPosSteps = 0;
int targetPosSteps = 0;
int membraneSizeSteps = 0;
int maxStrainSteps = 0;
int maxStrainRate_steps_per_sec = 0;
int strainCycles = 0;
int autoStepTime = 0;                 // received auto step time [ms]
int manualStepTime = 0;               // received manual step time [ms]
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

#define PUL_PIN 6
#define DIR_PIN 7
#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2
#define calibration_factor -260000

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


// --------- PROGRAM ---------

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  cb = createCircularBuffer(BUFFER_SIZE); // create circular buffer at cb location

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
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN, ledState);
  // delay(1000);

  // Serial.print("Reading: ");
  // Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  // Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  // Serial.println();

  checkSerialAvailable();
  newDataParsing();
  stepMotor();
  
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

        // parsing cmdByte0
        switch(cmdByte0){
          case 0:   // zero position
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            currentPosSteps = 0;
            break;
          case 1:   // move in +ve position
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            manualControl = true;
            manualStepTime = dataInt;
            break;
          case 2:   // move in -ve position
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            manualControl = true;
            manualStepTime = dataInt;
            break;
          case 3:   // set membrane size [steps]
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            membraneSizeSteps = dataInt;
            break;
          case 4:   // set strain target [steps]
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            targetPosSteps = dataInt;
            break;
          case 5:   // set strain rate [steps/second]
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            maxStrainRate_steps_per_sec = dataInt;
            break;
          case 6:   // set strain cycles
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            strainCycles = dataInt;
            break;
          case 7:   // set strain increment
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            break;
          case 8:   // stretch to max strain
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            targetPosSteps = maxStrainSteps;
            break;
          case 9:   // return to zero position
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            targetPosSteps = 0;
            break;
          case 10:  // cyclic stretching
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            autoControl = true;
            break;
          case 11:  // STOP STEPPER
            Serial.print("cmdByte0: ");
            Serial.println(cmdByte0);
            // stepper already disabled
            break;
          default:
            break;
        }

      }
    }

    newData = false;
  }
}



void stepMotor(){

}