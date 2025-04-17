// Define pin connections
const int waterSensorPin = 2; // Change this to the pin your SIG is connected to

void setup() {
  // Initialize the Serial Monitor
  Serial.begin(9600);
  
  // Set the water sensor pin as input
  pinMode(waterSensorPin, INPUT);
}

void loop() {
  // Read the sensor value
  int sensorValue = digitalRead(waterSensorPin);
  
  // Check if water is detected
  if (sensorValue == HIGH) {
    Serial.println("Water detected");
  } else {
    Serial.println("No water detected");
  }
  
  // Wait for a short period before checking again
  delay(1000); // Check every second
}
