int receivedChar;
bool newData;
int ledState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

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

  if(Serial.available() > 0){
    receivedChar = Serial.read();
    newData = true;
  }  

  if(newData){
    Serial.print("Received byte: ");
    Serial.println(receivedChar);
    if(receivedChar == '0')
      ledState = LOW;
    else if(receivedChar == '1')
      ledState = HIGH;
    newData = false;
  }
}
