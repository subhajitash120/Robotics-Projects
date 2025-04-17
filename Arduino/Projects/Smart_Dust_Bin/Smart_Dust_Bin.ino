#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3m_JICHMm"
#define BLYNK_TEMPLATE_NAME "Smart Dust Bin"
#define BLYNK_AUTH_TOKEN "0OK11Zk9ZETa2fPKAUiM2xk3qdHeLD0d"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
#include <Servo.h>

char ssid[] = "GANESH";
char pass[] = "Gani@4004";

#define TRIGGER_PIN_INSIDE 13
#define ECHO_PIN_INSIDE 15
#define TRIGGER_PIN_OUTSIDE 14
#define ECHO_PIN_OUTSIDE 12
#define MAX_DISTANCE 200
#define SERVO_PIN 0

NewPing sonarInside(TRIGGER_PIN_INSIDE, ECHO_PIN_INSIDE, MAX_DISTANCE);
NewPing sonarOutside(TRIGGER_PIN_OUTSIDE, ECHO_PIN_OUTSIDE, MAX_DISTANCE);
Servo myServo;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  myServo.attach(SERVO_PIN);
  timer.setInterval(1000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensorData() {
  unsigned int distanceInside = sonarInside.ping_cm();
  if (distanceInside == 0) {
    Serial.println("Inside Sensor Error: No echo received!");
  } else {
    int percentFull = mapToPercentage(distanceInside);
    Serial.print("Dustbin Full: ");
    Serial.print(percentFull);
    Serial.println("%");
    Blynk.virtualWrite(V0, percentFull);

    if (percentFull <= 20) {
      Blynk.logEvent("dust_bin_status");
    }
  }

  unsigned int distanceOutside = sonarOutside.ping_cm();
  if (distanceOutside == 0) {
    Serial.println("Outside Sensor Error: No echo received!");
    myServo.write(0);
  } else {
    Blynk.virtualWrite(V1, distanceOutside);
    if (distanceOutside < 20) {
      myServo.write(180);
      Serial.println("Dustbin lid open");
    } else {
      myServo.write(0);
      Serial.println("Dustbin lid closed");
    }
  }
}

int mapToPercentage(int distance) {
  if (distance <= 10) {
    return map(distance, 0, 10, 0, 100);
  } else {
    return 100;
  }
}
