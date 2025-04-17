#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3BKfeF8CL"
#define BLYNK_TEMPLATE_NAME "Home Safety System"
#define BLYNK_AUTH_TOKEN "7T3O4Y38XSrtO657DX4Izgb0eYX3zlRp"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

const int mq5DigitalPin = 14;
const int pirPin = 12;
const int thermistorPin = 13;
const int waterSensorPin = 15;
const int buzzerPin = 5;
const int ledPin = 4; // Define the LED pin

char ssid[] = "Yashu";
char pass[] = "9985996227";

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(mq5DigitalPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(thermistorPin, INPUT);
  pinMode(waterSensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT); // Initialize the LED pin as output
}

void loop() {
  Blynk.run();

  bool activateBuzzer = false;

  int mq5State = digitalRead(mq5DigitalPin);
  if (mq5State == LOW) {
    Serial.println("MQ-5 Sensor: Gas detected!");
    activateBuzzer = true;
    Blynk.virtualWrite(V0, 1); // Send '1' to indicate gas detected
    Blynk.logEvent("gases_detected");
  } else {
    Serial.println("MQ-5 Sensor: No gas detected.");
    Blynk.virtualWrite(V0, 0); // Send '0' to indicate no gas detected
  }

  int pirState = digitalRead(pirPin);
  if (pirState == HIGH) {
    Serial.println("PIR Sensor: Motion detected!");
    digitalWrite(ledPin, HIGH); // Turn on the LED
    Blynk.virtualWrite(V1, 1); // Send '1' to indicate motion detected
  } else {
    Serial.println("PIR Sensor: No motion detected.");
    digitalWrite(ledPin, LOW); // Turn off the LED
    Blynk.virtualWrite(V1, 0); // Send '0' to indicate no motion detected
  }

  int thermistorValue = digitalRead(thermistorPin);
  if (thermistorValue == LOW) {
    Serial.println("Fire detected by Thermistor Module!");
    activateBuzzer = true;
    Blynk.virtualWrite(V2, 1); // Send '1' to indicate fire detected
    Blynk.logEvent("fire_alarm");
  } else {
    Serial.println("No fire detected by Thermistor Module.");
    Blynk.virtualWrite(V2, 0); // Send '0' to indicate no fire detected
  }

  int waterValue = digitalRead(waterSensorPin);
  if (waterValue == HIGH) {
    Serial.println("Water detected by Water Sensor!");
    Blynk.virtualWrite(V3, 1); // Send '1' to indicate water detected
  } else {
    Serial.println("No water detected by Water Sensor.");
    Blynk.virtualWrite(V3, 0); // Send '0' to indicate no water detected
  }

  if (activateBuzzer) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  Serial.println();
  delay(1000);
}
