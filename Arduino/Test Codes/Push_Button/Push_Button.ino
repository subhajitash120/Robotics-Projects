// Include the necessary libraries
#include <ESP8266WiFi.h>

// Define the GPIO pin connected to the push button
const int buttonPin = 10;
// Define the GPIO pin connected to the relay
const int relayPin = 12;

// Variable to store the previous state of the button
int prevButtonState = HIGH;

// Variable to track the current state of the relay
bool relayState = false; // false indicates relay off, true indicates relay on

void setup() {
  // Set up serial communication
  Serial.begin(115200);

  // Set the button pin as input with pullup
  pinMode(buttonPin, INPUT_PULLUP);
  // Set the relay pin as output
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // Read the current state of the button
  int buttonState = digitalRead(buttonPin);

  // Check if the button state has changed
  if (buttonState != prevButtonState) {
    // If the button state has changed, toggle the relay state
    if (buttonState == LOW) {
      relayState = !relayState;
      
      // Toggle the relay
      digitalWrite(relayPin, relayState ? HIGH : LOW);
    }
    
    // Update the previous button state
    prevButtonState = buttonState;
  }

  // Add some delay to debounce the button
  delay(50);
}
