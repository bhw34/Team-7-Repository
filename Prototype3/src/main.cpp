/*
This protype opens the latch and door motor. This is done using 2 servos one for the latch, which opens first and one for the door which opens second.
*/

#include <Arduino.h>
#include <ESP32Servo.h>

// This initializes the output pins for both the servos
const int LATCH_SERVO_PIN = A1; // Latch Servo Pin
const int DOOR_SERVO_PIN = A0; // Door Servo Pin
const int sensorPin = A2; // Define the pin numbers for the Infrared obstacle avoidance sensor

// Initializes the servos
Servo latchServo;
Servo doorServo;

int closeAngle = 0; // Initializes int to store starting servo position
int openAngle = -120; // Initializes int to store open servo position


void setup() {
    Serial.begin(115200);
    pinMode(sensorPin, INPUT);  // Set sensorPin as input

    // Set independent hardware timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);

    // Set the frequency to 50Hz
    latchServo.setPeriodHertz(50);
    doorServo.setPeriodHertz(50);
    
    // Attach using the full parameter list (Pin, Min, Max)
    // Define the range for the ESP32 (500-2400 microseconds)
    if (!latchServo.attach(LATCH_SERVO_PIN, 500, 2400)) {
        Serial.println("Latch Servo failed to attach");
    }
    if (!doorServo.attach(DOOR_SERVO_PIN, 500, 2400)) {
        Serial.println("Door Servo failed to attach");
    }

    // Write start position
    latchServo.write(closeAngle);
    doorServo.write(closeAngle);
}

void loop() {
    delay(2000); // Delay, door cant open till latch is open
    latchServo.write(openAngle); // turns the latch servo 90 degrees

    Serial.println("Latch opened.");

    delay(2000); // Sets delay for robot cleaning cycle
    doorServo.write(openAngle); // turns the door servo 90 degrees

    Serial.println("Door opened.");
    
    Serial.println(digitalRead(sensorPin));  // Read the digital value from the sensor and print it to the serial monitor
    if (digitalRead(sensorPin) == LOW) { // Check if the sensor detects an obstacle
        Serial.println("Robot passed door.");
        doorServo.write(closeAngle); // Close the door if an obstacle is detected
        delay(2000); // Wait for a second before checking again
        latchServo.write(closeAngle); // Close the latch if an obstacle is detected
    }
}

