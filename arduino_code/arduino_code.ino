#include <SoftwareSerial.h>

SoftwareSerial xbee(10, 11);

String data = "";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  xbee.begin(9600);
  Serial.setTimeout(50);
}

void loop() {
  
  if(Serial.available()>0){ //if data has been written to the Serial stream
    
    char character = Serial.read();
    data.concat(character);

    if(character == 'f') {
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      for(int i = 0; i < data.length() - 1; i++) {
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(250);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(250);
      }
      delay(10000000000);
    }

    //testProcessData();
    //delay(1000000000000000);
       
  }
  
}

//transmit the entire string at once
void transmitData_1() {
  char charBuf[data.length() + 1];
  data.toCharArray(charBuf, data.length());
  xbee.write(charBuf);
}

//transmit one thing at a time
void transmitData_2() {
  for(int i = 0; i < data.length(); i++) {
    xbee.write(data.charAt(i));
  }
}

