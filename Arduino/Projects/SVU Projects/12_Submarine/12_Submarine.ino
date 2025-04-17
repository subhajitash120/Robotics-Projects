#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3HgkUB6GF"
#define BLYNK_TEMPLATE_NAME "SubMarine Safety SyStem"
#define BLYNK_AUTH_TOKEN "O_QZ7A68qJgxtISEglcbyUGhZlKXlqUB"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>

char ssid[] = "SVU";
char pass[] = "12345678";

#define TRIGGER_PIN 14
#define ECHO_PIN 12
#define MAX_DISTANCE 200

const int metalSensorPin = 5;
const int waterSensorPin = 4;
const int buzzerPin = 15;
const int mqSensorPin = 13; // Change from analog pin to digital pin 13

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

unsigned long previousMillis = 0;
unsigned long interval = 500;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(metalSensorPin, INPUT);
  pinMode(waterSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(mqSensorPin, INPUT); // Initialize the MQ sensor pin as input
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    unsigned int distance = sonar.ping_cm();
    if (distance == 0) {
      Serial.println("Error: No echo received!");
    } else {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
      Blynk.virtualWrite(V0, distance);

      if (distance < 30) {
        int buzzerTone = map(distance, 0, 30, 255, 0);
        analogWrite(buzzerPin, buzzerTone);
        Serial.print("Buzzer tone: ");
        Serial.println(buzzerTone);
      } else {
        digitalWrite(buzzerPin, LOW);
      }
    }

    int metalSensorValue = digitalRead(metalSensorPin);
    if (metalSensorValue == LOW) {
      Serial.println("Metal detected!");
      Blynk.virtualWrite(V1, HIGH);
      Blynk.logEvent("metal_detected");
    } else {
      Serial.println("No metal detected.");
      Blynk.virtualWrite(V1, LOW);
    }

    int mqSensorValue = digitalRead(mqSensorPin); // Read the value from the digital pin
    Serial.print("MQ Sensor Value: ");
    Serial.println(mqSensorValue);

    if (mqSensorValue == LOW) { // Assuming LOW means gas detected
      Serial.println("Harmful gas present!");
      Blynk.virtualWrite(V2, HIGH);
      Blynk.logEvent("gases_detected");
    } else {
      Serial.println("No harmful gas detected.");
      Blynk.virtualWrite(V2, LOW);
    }

    int waterSensorValue = digitalRead(waterSensorPin);
    if (waterSensorValue == HIGH) {
      Serial.println("Water detected!");
      Blynk.virtualWrite(V3, HIGH);
    } else {
      Serial.println("No water detected.");
      Blynk.virtualWrite(V3, LOW);
    }
  }
}
