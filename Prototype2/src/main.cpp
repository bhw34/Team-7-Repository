#include <Arduino.h>
#include <ESP32Servo.h>


const int LATCH_SERVO_PIN = A1; 
const int DOOR_SERVO_PIN = A0; 

Servo latchServo;
Servo doorServo;

int closeAngle = 0;
int openAngle = 90;

void setup() {
    Serial.begin(115200);

    // Force the ESP32 to set aside independent hardware timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);

    // 2. Set the frequency (Standard servos are 50Hz)
    latchServo.setPeriodHertz(50);
    doorServo.setPeriodHertz(50);

    if (!latchServo.attach(LATCH_SERVO_PIN, 500, 2400)) {
        Serial.println("Latch Servo failed to attach");
    }
    if (!doorServo.attach(DOOR_SERVO_PIN, 500, 2400)) {
        Serial.println("Door Servo failed to attach");
    }

    latchServo.write(closeAngle);
    doorServo.write(closeAngle);
}

void loop() {
    delay(2000);
    latchServo.write(openAngle); 
    
    delay(2000); 
    doorServo.write(openAngle); 

}