#include <Arduino.h>
#include <ESP32Servo.h>

// Define the pin numbers for the Infrared obstacle avoidance sensor
const int sensorPin = A0;

void setup() {
  pinMode(sensorPin, INPUT);  // Set sensorPin as input
  Serial.begin(115200);         // Start serial communication at 9600 baud rate
}

void loop() {
  // Reads 0 and light turns on if object is detected
  Serial.println(digitalRead(sensorPin));  // Read the digital value from the sensor and print it to the serial monitor
  delay(50);
}