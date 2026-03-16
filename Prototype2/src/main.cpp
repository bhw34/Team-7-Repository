#include <Arduino.h>
#include <ESP32Servo.h>

// This initializes the output pins for both the servos
const int LATCH_SERVO_PIN = A1; // Latch Servo Pin
const int DOOR_SERVO_PIN = A0; // Door Servo Pin

// Initializes the servos
Servo latchServo;
Servo doorServo;

int closeAngle = 0; // Initializes int to store starting servo position
int openAngle = 90; // Initializes int to store open servo position
bool doorOpen = false; // Door is closed



void setup() {
    Serial.begin(115200);
    Serial.begin(115200);

    // Set pin modes
    pinMode(LATCH_SERVO_PIN, OUTPUT);
    pinMode(DOOR_SERVO_PIN, OUTPUT);

    // Attach servos
    latchServo.attach(LATCH_SERVO_PIN);
    doorServo.attach(DOOR_SERVO_PIN);

    // Write start position
    latchServo.write(closeAngle);
    doorServo.write(closeAngle);

}

void loop() {

    latchServo.write(openAngle); // turns the latch servo 90 degrees

    delay(2000); // Delay, door cant open till latch is open
    
    doorServo.write(openAngle); // opens the door

    // doorOpen = true; // sets door condition to open

    delay(2000); // Sets delay for robot cleaning cycle
}