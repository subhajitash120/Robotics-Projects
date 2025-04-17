#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3uyoIIGUo"
#define BLYNK_TEMPLATE_NAME "Soldier Monitoring System"
#define BLYNK_AUTH_TOKEN "78FDlcVa--DoIKtWD-RkGKKUWIV7-JIa"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <NewPing.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

char ssid[] = "SVU";
char pass[] = "12345678";

const int metalSensorPin = 5;
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

#define TRIGGER_PIN 14
#define ECHO_PIN 12
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

SoftwareSerial gpsSerial(13, 15);  // RX, TX for GPS module
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(metalSensorPin, INPUT);
  dht.begin();
  gpsSerial.begin(9600);
}

void readAndSendMetalSensor() {
  int sensorValue = digitalRead(metalSensorPin);

  Blynk.virtualWrite(V0, sensorValue == LOW ? 1 : 0);
}

void readAndSendTemperature() {
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Blynk.virtualWrite(V1, temperature);
  }
}

void readAndSendDistance() {
  unsigned int distance = sonar.ping_cm();

  if (distance == 0) {
    Serial.println("Error: No echo received!");
    Blynk.virtualWrite(V2, -1);
  } else {
    Blynk.virtualWrite(V2, distance);
  }
}

void readAndSendGPS() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.satellites.value() > 1) {  // Changed to check for more than 3 satellites
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        Blynk.virtualWrite(V3, latitude);
        Blynk.virtualWrite(V4, longitude);

        // Print latitude and longitude on the serial monitor
        Serial.print("Latitude: ");
        Serial.println(latitude);
        Serial.print("Longitude: ");
        Serial.println(longitude);
      } else {
        Serial.println("Not enough satellites for an accurate fix");
      }
    }
  }
}

void loop() {
  Blynk.run();
  readAndSendMetalSensor();
  readAndSendTemperature();
  readAndSendDistance();
  readAndSendGPS();
}
