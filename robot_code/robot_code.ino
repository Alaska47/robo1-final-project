#include <Servo.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

int rotations = 0;

Servo R2, L1, L2, R1;

String data = "";

float offset = 0.0;

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
  offset = getOffset();
  while (command != 0)
  {   
      String com = String(command);
      if(com.indexOf(".") == -1) {
        Serial.print("Turning Angle: ");
        float angle = com.toInt() / 1.0;
        Serial.println(angle);
        float current_angle = getRobotAngle();
        float opt_angle = current_angle + angle;
        Serial.print("Current Angle: ");
        Serial.println(current_angle);
        Serial.print("Opt: ");
        Serial.println(opt_angle);
        float delta = 0.0;
        float previous_angle = current_angle;
        while(opt_angle > (previous_angle + 360.0 * rotations)) {
          current_angle = getRobotAngle();
          delta = abs(current_angle - previous_angle);
          if(delta > 180.0) {
            rotations += 1;
          }
          L1.write(156);
          R2.write(156);
          R1.write(156);
          L2.write(156);
          previous_angle = current_angle;
        }
        
      } else {
        Serial.print("Magnitude: ");
        float mag = com.toFloat();
        Serial.println(mag);
        
        R1.write(0);
        R2.write(0);
        L1.write(156);
        L2.write(156);
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

float getRobotAngle() {
  sensors_event_t event1;
  bno.getEvent(&event1);
  float current_angle = fmod(event1.orientation.x + offset, 360.0);
  return current_angle;
}


float getOffset() {
  sensors_event_t event1;
  bno.getEvent(&event1);
  return 361.0 - event1.orientation.x;
}
