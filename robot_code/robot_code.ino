#include <Servo.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);


Servo R2, L1, L2, R1;

String data = "";

void setup()
{
  // Set up both ports at 9600 baud. This value is most important
  // for the XBee. Make sure the baud rate matches the config
  // setting of your XBee.

  Serial.begin(9600);
  
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

  stopMotors();

}


void loop()
{ 
  if (Serial1.available() > 0)   //XBee/UART1/pins 0 and 1
  { // If data comes in from XBee, send it out to serial monitor
    Serial.println("reading data");
    data = Serial1.readString();
    Serial.println(data);
    testProcessData();
  }
  
  
}

void stopMotors() {
  L1.write(95);
  R2.write(94);
  R1.write(96);
  L2.write(96);
}


void testProcessData() {
  char charBuf[data.length() + 1];
  data.toCharArray(charBuf, data.length());
  char* command = strtok(charBuf, ",");
  while (command != 0)
  {   
      String com = String(command);
      if(com.indexOf(".") == -1) {
        Serial.print("Turning Angle: ");
        float angle = com.toInt() / 1.0;
        Serial.println(angle);
        
        sensors_event_t event;
        bno.getEvent(&event);
        float current_angle = fmod(event.orientation.x + 5.0, 360.0);
        float opt_angle = current_angle + angle + 5.0;
        Serial.print("Current Angle: ");
        Serial.println(current_angle);
        Serial.print("Opt: ");
        Serial.println(opt_angle);
        float delta = 0.0;
        while(opt_angle > current_angle) {
          sensors_event_t event1;
          bno.getEvent(&event1);
          float new_current_angle = event1.orientation.x;
          delta = new_current_angle - current_angle;
          L1.write(180);
          R2.write(180);
          R1.write(180);
          L2.write(180);
          sensors_event_t event1;
          bno.getEvent(&event1);
          current_angle = fmod(event1.orientation.x + 5.0, 360.0);
          
          Serial.println(current_angle);
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
      L1.write(95);
      R2.write(94);
      R1.write(96);
      L2.write(96);
      delay(250);  
      // Find the next command in input string
      command = strtok(0, ",");
  }
}

