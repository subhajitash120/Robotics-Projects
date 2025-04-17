#include <ESP8266WiFi.h>

// Define the pin for the metal sensor
const int sensorPin = 5; // GPIO 5

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  
  // Set the sensor pin as input
  pinMode(sensorPin, INPUT);
}

void loop() {
  // Read the sensor value
  int sensorValue = digitalRead(sensorPin);
  
  // Check if metal is detected (assuming LOW means metal detected)
  if (sensorValue == LOW) {
    Serial.println("Metal detected!");
  } else {
    Serial.println("No metal detected.");
  }
  
  // Add a small delay to avoid flooding the serial monitor
  delay(500);
}
