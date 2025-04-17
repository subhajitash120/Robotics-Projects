// Define the analog pin connected to the MQ-5 sensor
const int analogPin = A0;

void setup() {
  // Start the serial communication
  Serial.begin(115200);
}

void loop() {
  // Read the analog value from the MQ-5 sensor
  int sensorValue = analogRead(analogPin);

  // Convert the analog value to a voltage (ESP8266 ADC range is 0-1V by default)
  float voltage = sensorValue * (1.0 / 1023.0);

  // Print the sensor value and voltage to the Serial Monitor
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print(" - Voltage: ");
  Serial.println(voltage);

  // Add a delay for better readability
  delay(1000);
}
