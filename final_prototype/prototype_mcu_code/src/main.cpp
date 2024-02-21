#include <Arduino.h>
#include "HX711.h"

int x = 0;
int y;

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
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

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}