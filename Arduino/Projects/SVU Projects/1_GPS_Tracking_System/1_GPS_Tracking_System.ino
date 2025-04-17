#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3435jBUiT"
#define BLYNK_TEMPLATE_NAME "GPS Tracking system"
#define BLYNK_AUTH_TOKEN "9-y3UbAxi6WfzRD4_CgokohNA9myxjbv"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

const int buttonPin = 5;

char ssid[] = "SVU";
char pass[] = "12345678";

SoftwareSerial gpsSerial(13, 15);
TinyGPSPlus gps;

int updateInterval = 1000;
unsigned long lastEventTrigger = 0;
int lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  readGPSData();
  triggerEvent();
  checkButton();
}

BLYNK_WRITE(V3) {
  int sliderValue = param.asInt();
  updateInterval = sliderValue * 60000;
  Serial.print("Update interval set to: ");
  Serial.print(updateInterval / 1000);
  Serial.println(" seconds");
}

void readGPSData() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.satellites.value() > 1) {
        double latitude = gps.location.lat();
        double longitude = gps.location.lng();
        double speed = gps.speed.kmph();

        Serial.print("Latitude: ");
        Serial.println(latitude, 6);
        Serial.print("Longitude: ");
        Serial.println(longitude, 6);
        Serial.print("Speed (km/h): ");
        Serial.println(speed, 2);
        Serial.println();

        Blynk.virtualWrite(V1, latitude);
        Blynk.virtualWrite(V2, longitude);
        Blynk.virtualWrite(V0, speed);
      } else {
        Serial.println("Not enough satellites for an accurate fix");
      }
    }
  }
}

void triggerEvent() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastEventTrigger >= updateInterval) {
    lastEventTrigger = currentMillis;
    Serial.println("Triggering Blynk event: time_fencing");
    Blynk.logEvent("time_fencing");
  }
}

void checkButton() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    Serial.println("Button pressed");
    Blynk.logEvent("sos_alert");
  }
  lastButtonState = buttonState;
}
