
/*
This code makes use of both the previous prototypes to implement an iterative more complete design.
For this code the code from protype 2 will be used to open the doors after a certain time elapsed. Then when the 
ir sensor is moved near an object, simulating the robot crossing the threshold, the door will close and a trigger 
will be set to remember that this door has been opened and closed
*/
#include <Arduino.h>
#include <ESP32Servo.h>

const int sensorPin = A2; // Configures the IR sensor pin
bool doorPassed = false; // Trigger to set door condition

void setup() {
  pinMode(sensorPin, INPUT);  // Set sensorPin as input
  Serial.begin(115200);         // Start serial communication 
}

void loop() {
  // If the sensor pin is equal to 1 close the door and set the trigger to true
  if(digitalRead(sensorPin) == 1) {

  }
}