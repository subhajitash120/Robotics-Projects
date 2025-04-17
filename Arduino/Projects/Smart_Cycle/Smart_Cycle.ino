#define BLYNK_PRINT Serial  // Define BLYNK_PRINT to use Serial for debug prints

#define BLYNK_TEMPLATE_ID "TMPL38nkNdEys"  // Template ID for Blynk project
#define BLYNK_TEMPLATE_NAME "Smart Cycle"  // Name of the Blynk template
#define BLYNK_AUTH_TOKEN "2Td89Zhyvyg70BNb7pBxi8ukkjPaX0Qk"  // Authentication token for Blynk

#include <NewPing.h>  // Include NewPing library for ultrasonic sensor
#include <ESP8266WiFi.h>  // Include ESP8266WiFi library for ESP8266 WiFi module
#include <BlynkSimpleEsp8266.h>  // Include Blynk library for ESP8266

#include <DHT.h>  // Include DHT library for DHT temperature and humidity sensor

char ssid[] = "Cookie";  // WiFi SSID
char pass[] = "123456780";  // WiFi password

#define DHTPIN 4  // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT sensor type (DHT11 in this case)

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor

#define TRIGGER_PIN_TRUNK 14  // Digital pin connected to the trigger pin of the trunk ultrasonic sensor
#define ECHO_PIN_TRUNK 12  // Digital pin connected to the echo pin of the trunk ultrasonic sensor

#define TRIGGER_PIN_FRUNK 5  // Digital pin connected to the trigger pin of the frunk ultrasonic sensor
#define ECHO_PIN_FRUNK 16  // Digital pin connected to the echo pin of the frunk ultrasonic sensor

#define BUZZER_PIN 13  // Digital pin connected to the buzzer

#define MAX_DISTANCE 200  // Maximum distance for the ultrasonic sensors

NewPing trunkSonar(TRIGGER_PIN_TRUNK, ECHO_PIN_TRUNK, MAX_DISTANCE);  // Initialize trunk ultrasonic sensor
NewPing frunkSonar(TRIGGER_PIN_FRUNK, ECHO_PIN_FRUNK, MAX_DISTANCE);  // Initialize frunk ultrasonic sensor

void setup() {
  Serial.begin(115200);  // Begin serial communication at 115200 baud rate
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // Connect to Blynk server with provided authentication token and WiFi credentials
  pinMode(BUZZER_PIN, OUTPUT);  // Set BUZZER_PIN as output
  dht.begin();  // Initialize DHT sensor
}

void loop() {
  Blynk.run();  // Run Blynk

  float temperature = dht.readTemperature();  // Read temperature from DHT sensor

  if (isnan(temperature)) {  // Check if temperature reading is NaN
    Serial.println("Failed to read from DHT sensor!");  // Print error message
    return;
  }

  Serial.print("Temperature: ");  // Print temperature label
  Serial.print(temperature);  // Print temperature value
  Serial.println(" °C");  // Print units
  Blynk.virtualWrite(V0, temperature);  // Send temperature value to Blynk app

  if (temperature > 40) {  // Check if temperature is above 40°C
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
    Blynk.logEvent("batter_tempeature");  // Log temperature event in Blynk app
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
  }

  unsigned int frunkDistance = frunkSonar.ping_cm();  // Get distance from frunk ultrasonic sensor
  unsigned int trunkDistance = trunkSonar.ping_cm();  // Get distance from trunk ultrasonic sensor

  if (frunkDistance == 0) {  // Check if frunk distance reading is 0
    Serial.println("Frunk Error: No echo received!");  // Print error message
  } else {
    Serial.print("Frunk Distance: ");  // Print frunk distance label
    Serial.print(frunkDistance);  // Print frunk distance value
    Serial.println(" cm");  // Print units
    Blynk.virtualWrite(V1, frunkDistance);  // Send frunk distance value to Blynk app
  }

  if (frunkDistance <= 30) {  // Check if distance to frunk is less than or equal to 10 cm
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
    Blynk.logEvent("vehicle_ahead");  // Log vehicle ahead event in Blynk app
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
  }

  if (trunkDistance == 0) {  // Check if trunk distance reading is 0
    Serial.println("Trunk Error: No echo received!");  // Print error message
  } else {
    Serial.print("Trunk Distance: ");  // Print trunk distance label
    Serial.print(trunkDistance);  // Print trunk distance value
    Serial.println(" cm");  // Print units
    Blynk.virtualWrite(V2, trunkDistance);  // Send trunk distance value to Blynk app
  }

  if (trunkDistance <= 30) {  // Check if distance to trunk is less than or equal to 10 cm
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
    Blynk.logEvent("vehicle_behind");  // Log vehicle behind event in Blynk app
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
  }

  Serial.println();  // Print empty line for readability
  delay(1000);  // Delay for 1 second
}
