#include <Servo.h>  // Include the Servo library

Servo myServo;  // Create a Servo object

int servoPin = 0;  // Define the pin the servo is connected to

void setup() {
  myServo.attach(servoPin);  // Attach the servo to the specified pin
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 10) {  // Sweep from 0 to 180 degrees
    myServo.write(angle);  // Set the servo to the current angle
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(500);  // Wait for half a second
  }

  for (int angle = 180; angle >= 0; angle -= 10) {  // Sweep from 180 to 0 degrees
    myServo.write(angle);  // Set the servo to the current angle
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(500);  // Wait for half a second
  }
}
