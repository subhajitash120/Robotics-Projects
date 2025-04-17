#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3i30mwo6x"                    // Blynk template ID
#define BLYNK_TEMPLATE_NAME "Smart Water Bottle"             // Blynk template name
#define BLYNK_AUTH_TOKEN "ZbdgivudstZ6ohaNJAujAizKID_YsMTS"  // Blynk authentication token
#define sensorPin A0                                         // Analog pin A0 for water level sensor

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char ssid[] = "Vivekananda";  // Wi-Fi SSID
char pass[] = "Vivekananda";  // Wi-Fi password

// Data wire is plugged into port 2 on the ESP8266
#define ONE_WIRE_BUS 2

// Setup a OneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Variable to store the previous state of V4
bool previousV4State = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // Initialize Blynk
  delay(1000);
  sensors.begin();  // Initialize the temperature sensor
}

void loop() {
  Blynk.run();
  sensors.requestTemperatures();             // Request temperature from sensor
  int sensorValue = analogRead(sensorPin);   // Read water level sensor value
  float tempC = sensors.getTempCByIndex(0);  // Get temperature in Celsius

  if (tempC != DEVICE_DISCONNECTED_C) {
    Blynk.virtualWrite(V0, tempC);  // Send temperature to virtual pin V0
  } else {
    Serial.println("Error: Could not read temperature data");  // Error message for temperature reading failure
  }

  // Water level logic
  if (sensorValue < 150) {
    Blynk.virtualWrite(V1, 1);     // Low water level indication
    Blynk.virtualWrite(V2, 0);     // Medium water level indication
    Blynk.virtualWrite(V3, 0);     // High water level indication
    Blynk.logEvent("low_water_");  // Log low water event
  } else if (sensorValue >= 150 && sensorValue <= 300) {
    Blynk.virtualWrite(V1, 0);  // Low water level indication
    Blynk.virtualWrite(V2, 1);  // Medium water level indication
    Blynk.virtualWrite(V3, 0);  // High water level indication
  } else {
    Blynk.virtualWrite(V1, 0);  // Low water level indication
    Blynk.virtualWrite(V2, 0);  // Medium water level indication
    Blynk.virtualWrite(V3, 1);  // High water level indication
  }
}

BLYNK_WRITE(V4) {
  bool currentV4State = param.asInt();  // Read the state of virtual pin V4
  if (currentV4State == 1 && !previousV4State) {
    Blynk.logEvent("drink_water");  // Log drink water event
  }
  previousV4State = currentV4State;  // Update previous state
}
