#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into GPIO 2 on the ESP8266
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1, sensor2;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  // Start the DS18B20 sensor library
  sensors.begin();

  // Locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // Get the addresses of the sensors
  if (!sensors.getAddress(sensor1, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(sensor2, 1)) Serial.println("Unable to find address for Device 1");

  // Show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  printAddress(sensor1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(sensor2);
  Serial.println();
}

void loop() {
  // Request temperature readings
  sensors.requestTemperatures();

  // Fetch and print the temperature for each sensor
  float tempC1 = sensors.getTempC(sensor1);
  float tempC2 = sensors.getTempC(sensor2);

  Serial.print("Temperature for Device 0: ");
  Serial.print(tempC1);
  Serial.println("°C");

  Serial.print("Temperature for Device 1: ");
  Serial.print(tempC2);
  Serial.println("°C");

  // Wait 2 seconds before repeating
  delay(2000);
}

// Function to print the device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
