#define PUL_PIN 6
#define DIR_PIN 7
#define ENA_PIN 8

#define positive 0
#define negative 1
#define disabled 1
#define enabled 0

// definiting variables
bool stepState = false;
bool ledState = negative;
bool dirState = negative;

// define bit constants
int BIT0 = 0x0001;
int BIT1 = 0x0002;
int BIT2 = 0x0004;
int BIT3 = 0x0008;

/*
Calculations (for 500ms): 
  System clock 16 Mhz and Prescalar 1024;
  Timer 1 speed = 16Mhz/1024 = 15.625 Khz    
  Pulse time = 1/15.625 Khz =  64us  
  Count up to = 500ms / 64us = 31250 (so this is the value the OCR register should have)


  System clock 16 Mhz and Prescalar 256;
  Timer 1 speed = 16Mhz/256 = 62.5 Khz    
  Pulse time = 1/62.5 Khz =  16us  
  Count up to = 10ms / 16us = 625 (so this is the value the OCR register should have)


  System clock 16 Mhz and Prescalar 8;
  Timer 1 speed = 16Mhz/8 = 2 Mhz    
  Pulse time = 1/2 Mhz =  0.5us  
  Count up to = 0.5ms / 0.5us = 1000 (so this is the value the OCR register should have)
  This means 1 step every 1 ms
*/

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  digitalWrite(ENA_PIN, enabled);

  cli();  // interrupt disable

  // timer 1 (interrupt each 500ms)
  TCCR1A = 0;
  TCCR1B = 0;

  // TCCR1B |= BIT2 + BIT0; // clock pre-scaler 1024

  // TCCR1B |= BIT2; // clock pre-scaler 256

  TCCR1B |= BIT1; // clock pre-scaler 8

  TIMSK1 |= BIT1; // compare select on OCR1A

  OCR1A = 1000;  // compare value for compare

  sei();  // interrupt enable
}

void loop() {
  // put your main code here, to run repeatedly:

}

ISR(TIMER1_COMPA_vect){
  TCNT1 = 0; // reset timer to 0
  stepState = !stepState; // swap the step state
  ledState = !ledState;
  digitalWrite(DIR_PIN, dirState);
  digitalWrite(PUL_PIN, stepState);
  digitalWrite(LED_BUILTIN, ledState);
}