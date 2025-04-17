#include <DHT.h>

// Define the type of sensor and the pin it's connected to
#define DHTPIN 4     // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT 11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200); // Initialize serial communication
  dht.begin();        // Initialize DHT sensor
}

void loop() {
  // Read temperature as Celsius
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(2000); // Delay between readings
}
