//#include "DFRobot_HX711.h"
#include "HX711.h"

byte receivedChar;
bool newData;
int ledState = 0;

#define LOADCELL_DOUT_PIN  3
#define LOADCELL_SCK_PIN  2
#define calibration_factor -260000

HX711 scale;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

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

  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  if(Serial.available() > 0){
    receivedChar = Serial.read();
    newData = true;
  }  

  if(newData){
    // Serial.print("Received byte: ");
    Serial.write(receivedChar);
    // Serial.println("");
    if(receivedChar == 0)
      ledState = LOW;
    else if(receivedChar == 1)
      ledState = HIGH;
    newData = false;
  }
}