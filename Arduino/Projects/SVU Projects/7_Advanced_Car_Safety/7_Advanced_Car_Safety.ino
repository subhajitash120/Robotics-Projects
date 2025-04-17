#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3tYH7J3fx"
#define BLYNK_TEMPLATE_NAME "Advanced Car Safety"
#define BLYNK_AUTH_TOKEN "Rx5QrJv87Yids1oH5SAGaTLovjrHUcdA"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char ssid[] = "SVU";
char pass[] = "12345678";

#define DOOR_LOCK_PIN 5
#define ENGINE_PIN 4
#define SEAT_BELT_PIN 14

Servo myServo;
int servoPin = 0;
bool servoActive = false;
int buttonState = 0;
int servoPos = 0;
bool servoIncreasing = true;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(DOOR_LOCK_PIN, OUTPUT);
  pinMode(ENGINE_PIN, OUTPUT);
  pinMode(SEAT_BELT_PIN, INPUT_PULLUP);

  digitalWrite(DOOR_LOCK_PIN, LOW);
  digitalWrite(ENGINE_PIN, LOW);

  myServo.attach(servoPin);
  myServo.write(0);
}

BLYNK_WRITE(V1) {
  int doorLockState = param.asInt();
  digitalWrite(DOOR_LOCK_PIN, doorLockState);
}

BLYNK_WRITE(V2) {
  int engineState = param.asInt();
  digitalWrite(ENGINE_PIN, engineState);
}

BLYNK_WRITE(V0) {
  int pinValue = param.asInt();
  if (pinValue == 1) {
    servoActive = true;
  } else {
    servoActive = false;
    myServo.write(90);
  }
}

void updateServo() {
  if (servoActive) {
    if (servoIncreasing) {
      servoPos += 3;
      if (servoPos >= 180) {
        servoIncreasing = false;
      }
    } else {
      servoPos -= 3;
      if (servoPos <= 0) {
        servoIncreasing = true;
      }
    }
    myServo.write(servoPos);
  }
}

void loop() {
  Blynk.run();
  buttonState = digitalRead(SEAT_BELT_PIN);
  int invertedButtonState = !buttonState;
  Blynk.virtualWrite(V3, invertedButtonState);
  updateServo();
  delay(20);
}
