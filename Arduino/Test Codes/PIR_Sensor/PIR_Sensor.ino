// Define the pin number where your sensor is connected
const int sensorPin = 14; // Change this to the appropriate pin number

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set sensor pin as input
  pinMode(sensorPin, INPUT);
}

void loop() {
  // Read the state of the sensor
  int sensorState = digitalRead(sensorPin);

  // Print the sensor state to the serial monitor
  Serial.print("Sensor state: ");
  Serial.println(sensorState);

  // Wait a short delay before reading again
  delay(1000); // Adjust delay time as needed
}
