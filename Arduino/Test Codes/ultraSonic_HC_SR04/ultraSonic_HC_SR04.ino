#include <NewPing.h>

#define TRIGGER_PIN 14  // NodeMCU pin for trigger (connected to sensor's trigger pin)
#define ECHO_PIN 12     // NodeMCU pin for echo (connected to sensor's echo pin)
#define MAX_DISTANCE 200 // Maximum distance in centimeters

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(500); // Delay for stability

  unsigned int distance = sonar.ping_cm(); // Get distance in centimeters

  if (distance == 0) {
    Serial.println("Error: No echo received!");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
