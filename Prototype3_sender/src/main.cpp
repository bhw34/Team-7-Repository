/*
This protype opens the latch and door motor. This is done using 2 servos one for the latch, which opens first and one for the door which opens second.
*/

#include <Arduino.h>
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

// This initializes the output pins for both the servos
const int LATCH_SERVO_PIN = A1; // Latch Servo Pin
const int DOOR_SERVO_PIN = A0; // Door Servo Pin
const int sensorPin = A2; // Define the pin numbers for the Infrared obstacle avoidance sensor

// Initializes the servos
Servo latchServo;
Servo doorServo;

int closeAngle = 0; // Initializes int to store starting servo position
int openAngle = 90; // Initializes int to store open servo position

bool flag = true;

// REPLACE WITH YOUR RECEIVER MAC Address
// ESP 24: 00:4b:12:be:cf:38
// ESP 28: f4:65:0b:33:52:e4

uint8_t broadcastAddress[] = {0x00, 0x4B, 0x12, 0xBE, 0xCF, 0x38};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
// define what data type needs to be sent
  bool door;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo; // what is this peerInfo?

// this function runs when a message is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

    // Write start position
    latchServo.write(closeAngle);
    doorServo.write(closeAngle);

    // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}

void loop() {
    if (flag) {
        flag = false;
        delay(5000); // Delay, door cant open till latch is open
        latchServo.write(openAngle); // turns the latch servo 90 degrees

        delay(2000); // Sets delay for robot cleaning cycle
        doorServo.write(openAngle); // turns the door servo 90 degrees

    }
        Serial.println(digitalRead(sensorPin));  // Read the digital value from the sensor and print it to the serial monitor
    if (digitalRead(sensorPin) == LOW) { // Check if the sensor detects an obstacle
        // Serial.println("Robot passed door.");
        delay(4000);
        doorServo.write(closeAngle); // Close the door if an obstacle is detected
        delay(2000); // Wait for a second before checking again
        latchServo.write(closeAngle); // Close the latch if an obstacle is detected
        flag = true;
        
        myData.door = true;

        // Send message via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
         if (result == ESP_OK) {
             Serial.println("Sent with success");
         }
        else {
            Serial.println("Error sending the data");
        }
        delay(2000);
     }

    
}