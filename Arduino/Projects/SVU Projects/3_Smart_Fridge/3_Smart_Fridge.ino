#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3AFErITak"
#define BLYNK_TEMPLATE_NAME "Smart Fridge with Vital Monitoring Capabilities"
#define BLYNK_AUTH_TOKEN "ZX2xBT2WmVfKByXkoebHImYld6LZ7Uyk"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin connected to the battery voltage sensor
int analogInPin = A0;
int sensorValue;
float calibration = 0.21;  // Calibration factor for voltage reading
int bat_percentage;        // Battery percentage

// Wi-Fi credentials
char ssid[] = "SVU";
char pass[] = "12345678";

// Pin connected to the DS18B20 temperature sensors
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress sensor1, sensor2;

void setup() {
  // Initialize serial communication at 9600 baud rate
  Serial.begin(9600);

  // Initialize Blynk with authentication token and Wi-Fi credentials
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Start the DS18B20 sensor library
  sensors.begin();

  // Locate devices on the bus and print the count
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // Get the addresses of the sensors and print them
  if (!sensors.getAddress(sensor1, 0)) Serial.println("Unable to find address for Device 0");
  if (!sensors.getAddress(sensor2, 1)) Serial.println("Unable to find address for Device 1");

  // Print the addresses of the sensors
  Serial.print("Device 0 Address: ");
  printAddress(sensor1);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(sensor2);
  Serial.println();
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read battery voltage and calculate percentage
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration);
  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100);

  // Constrain battery percentage to valid range
  if (bat_percentage >= 100) {
    bat_percentage = 100;
  } else if (bat_percentage <= 0) {
    bat_percentage = 1;
  }

  // Send battery voltage and percentage to Blynk
  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, bat_percentage);

  // Request temperatures from sensors
  sensors.requestTemperatures();
  float tempC1 = sensors.getTempC(sensor1);
  float tempC2 = sensors.getTempC(sensor2);

  // Send temperatures to Blynk
  Blynk.virtualWrite(V2, tempC1);
  Blynk.virtualWrite(V3, tempC2);
}

// Function to map float values from one range to another
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Function to print the device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
