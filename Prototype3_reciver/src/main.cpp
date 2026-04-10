/*
This protype opens the latch and door motor. This is done using 2 servos one for the latch, which opens first and one for the door which opens second.
*/

#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

// This initializes the output pins for both the servos
const int LATCH_SERVO_PIN = A0; // Latch Servo Pin
const int DOOR_SERVO_PIN = A1; // Door Servo Pin
const int sensorPin = A2; // Define the pin numbers for the Infrared obstacle avoidance sensor

// Initializes the servos
Servo latchServo;
Servo doorServo;

int closeAngle = 0; // Initializes int to store starting servo position
int openAngle = 90; // Initializes int to store open servo position

bool flag = true;
unsigned long startTime = 0;
const unsigned long delayTime = 25000;
bool stopLoop = true;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    bool door;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Received");
  delay(10000);
  stopLoop = false;
  
}

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

    startTime = millis();
    // Write start position
    doorServo.write(closeAngle);
    delay(2000);
    latchServo.write(openAngle);

    // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

}

void loop() {
    while (!stopLoop) {
        if (flag || (millis() - startTime >= delayTime)) {
            flag = false;
            Serial.println("Time:" + String(millis() - startTime));

            delay(5000); // Delay, door cant open till latch is open
            latchServo.write(closeAngle); // turns the latch servo 90 degrees

            delay(2000); // Sets delay for robot cleaning cycle
            doorServo.write(openAngle); // turns the door servo 90 degrees

            if (millis() - startTime >= delayTime) {
                Serial.println("delay time reached");
            }
            delay(1000);
        }
        
        Serial.println("Sensor Value: " + String(digitalRead(sensorPin)));  // Read the digital value from the sensor and print it to the serial monitor
        if (digitalRead(sensorPin) == LOW || (millis() - startTime >= delayTime)) { // Check if the sensor detects an obstacle
            Serial.println("Robot passed door.");
            delay(4000);
            doorServo.write(closeAngle); // Close the door if an obstacle is detected
            delay(2000); // Wait for a second before checking again
            latchServo.write(openAngle); // Close the latch if an obstacle is detected
            flag = true;
            delay(5000);
        }

        if (millis() - startTime >= delayTime) {
            stopLoop = true;
        }
    }
}
