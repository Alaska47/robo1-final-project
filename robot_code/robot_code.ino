#include <Servo.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);


Servo R2, L1, L2, R1;

sensors_event_t event;

String data = "";

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

  R2.attach(10);
  L1.attach(8);
  R1.attach(6);
  L2.attach(4);
  
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
          L1.write(180);
          R2.write(180);
          R1.write(180);
          L2.write(180);
          bno.getEvent(&event);
        }
      } else {
        Serial.print("Magnitude: ");
        float mag = com.toFloat();
        Serial.println(mag);
        R1.write(0);
        R2.write(0);
        L1.write(180);
        L2.write(180);
        delay(mag * 1000);
      }
      L1.write(96);
      R2.write(96);
      R1.write(96);
      L2.write(96);
      delay(250);  
      // Find the next command in input string
      command = strtok(0, ",");
  }
}
