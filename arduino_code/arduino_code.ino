#include <SoftwareSerial.h>

SoftwareSerial xbee(10, 11);

String data = "180,1.03,111,1.01,123,1.15f";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  
  transmitData_1();
  while(1);
  
}

//transmit the entire string at once
void transmitData_1() {
  xbee.println(data);
}

//transmit one thing at a time
void transmitData_2() {
  for(int i = 0; i < data.length(); i++) {
    Serial.println(data.charAt(i));
    xbee.write(data.charAt(i));
  }
}

