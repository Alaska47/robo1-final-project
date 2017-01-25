#include <Servo.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);


Servo front1, front2, back1, back2;

sensors_event_t event;

String data = "";
/*
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  while(!Serial1){
    ;
  }
  
  front1.attach(4);
  front2.attach(6);
  back1.attach(8);
  back2.attach(10);
  pinMode(led, OUTPUT);
 
}

void loop() {
  
  
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    Serial.println(c);    
  }
  
  front1.write(136);
  front2.write(56);
  back1.write(136);
  back2.write(56);
  
}

*/

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.

  Serial.begin(9600);

  while(!Serial) {
    ;
  }
  
  Serial.println("Orientation Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  bno.setExtCrystalUse(true);
  
  Serial1.begin(9600);  //XBee/UART1/pins 0 and 1

}


void loop()
{ 
  bno.getEvent(&event);
  if (Serial1.available() > 0)   //XBee/UART1/pins 0 and 1
  { // If data comes in from XBee, send it out to serial monitor
    char c = Serial1.read();
    data.concat(c);  //Serial port
    if(c == 'f') {

      testProcessData();
      
    }
  }
  
}


void testProcessData() {
  char charBuf[data.length() + 1];
  data.toCharArray(charBuf, data.length());
  char* command = strtok(charBuf, ",");
  while (command != 0)
  {
      String com = String(command);
      if(com.indexOf(".") == -1) {
        Serial.print("Angle: ");
        float angle = com.toInt() / 1.0;
        Serial.println(angle);
        float current_angle = event.orientation.x;
        float opt_angle = current_angle - angle;
        while(opt_angle > event.orientation.x) {
          //turn right
          bno.getEvent(&event);
        }
      } else {
        Serial.print("Magnitude: ");
        float mag = com.toFloat();
        Serial.println(mag);
        //go forward for mag * 1000 
      }
      delay(250);  
      // Find the next command in input string
      command = strtok(0, ",");
  }
}
