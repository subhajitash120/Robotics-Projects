#include <Servo.h>

// Define the digital pin to which the sensor is connected
const int sensorPin = 2; // Change this to the actual digital pin number
const int servoPin = 3;  // Change this to the actual digital pin number for the servo

Servo myServo;

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  pinMode(sensorPin, INPUT);  // Set the sensor pin as INPUT
  myServo.attach(servoPin);  // Attach the servo to its pin
}

void loop() {
  // Read the digital value from the sensor
  int sensorValue = digitalRead(sensorPin);

  // Print the sensor value to the serial monitor
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);

  // Control the servo based on the moisture level
  if (sensorValue == HIGH) {
    rotateServo(90);  // Rotate servo to 90 degrees when moist
  } else {
    rotateServo(0);   // Rotate servo to 0 degrees when dry
  }

  // Add a delay to make the output more readable
  delay(1000);  // Adjust the delay time as needed
}

void rotateServo(int angle) {
  int currentAngle = myServo.read();
  while (currentAngle != angle) {
    if (currentAngle < angle) {
      currentAngle += 2;  // Increment angle by 2 degrees
      if (currentAngle > angle) {
        currentAngle = angle;
      }
    } else {
      currentAngle -= 2;  // Decrement angle by 2 degrees
      if (currentAngle < angle) {
        currentAngle = angle;
      }
    }
    myServo.write(currentAngle);
    delay(200);  // Adjust the delay time as needed for the servo to move 2 degrees per second
  }
}
