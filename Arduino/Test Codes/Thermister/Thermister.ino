// Define the digital input pin for the thermistor module
const int thermistorPin = 2; // Adjust the pin number as needed

void setup() {
  // Initialize the digital pin as an input:
  pinMode(thermistorPin, INPUT);
  
  // Begin serial communication at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // Read the digital input from the thermistor module:
  int sensorValue = digitalRead(thermistorPin);
  
  // Check if the sensor detects fire:
  if (sensorValue == HIGH) {
    // If fire is detected, print a message to the serial monitor:
    Serial.println("Fire detected!");
  } else {
    // If no fire is detected, print a different message:
    Serial.println("No fire detected.");
  }
  
  // Add a small delay before the next loop:
  delay(100);
}
S