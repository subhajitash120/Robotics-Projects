// Soil Moisture Sensor Example
int soilMoisturePin = 5;  // Replace 2 with the digital pin connected to the sensor

void setup() {
  Serial.begin(9600); // Start the serial communication
}

void loop() {
  int soilMoistureValue = digitalRead(soilMoisturePin);

  // Print the soil moisture status to the serial monitor
  if (soilMoistureValue == HIGH) {
    Serial.println("Soil is dry");
  } else {
    Serial.println("Soil is wet");
  }

  delay(1000); // Delay for 1 second before reading again
}
