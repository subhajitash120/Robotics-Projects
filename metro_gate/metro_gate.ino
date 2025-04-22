#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo gateServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int greenLED = 6;
const int redLED = 7;
const int buzzerPin = 5;
const int servoPin = 3;

String allowedUID = "3f636a6";
String rejectedUID = "22d47053";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  gateServo.attach(servoPin);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gate is Closed");
  gateServo.write(0); // Closed
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }
  tagID.toLowerCase();
  Serial.println("Tag ID: " + tagID);

  if (tagID == allowedUID) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print("Gate Opened");

    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    stylishTone();
    gateServo.write(90);

    delay(3000);

    gateServo.write(0);
    digitalWrite(greenLED, LOW);
    beepOnce();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gate is Closed");

  } else if (tagID == rejectedUID) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    lcd.setCursor(0, 1);
    lcd.print("Gate Closed");

    digitalWrite(greenLED, LOW);
    blinkRedLED();
    rejectionSound();

    gateServo.write(0);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Gate is Closed");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void stylishTone() {
  tone(buzzerPin, 1000, 200);
  delay(250);
  tone(buzzerPin, 1200, 200);
  delay(250);
  tone(buzzerPin, 1500, 300);
  delay(300);
  noTone(buzzerPin);
}

void beepOnce() {
  tone(buzzerPin, 1000, 100);
  delay(150);
  noTone(buzzerPin);
}

void blinkRedLED() {
  for (int i = 0; i < 6; i++) {
    digitalWrite(redLED, HIGH);
    delay(random(100, 200));
    digitalWrite(redLED, LOW);
    delay(random(100, 200));
  }
}

void rejectionSound() {
  tone(buzzerPin, 400, 200);
  delay(200);
  tone(buzzerPin, 300, 200);
  delay(200);
  tone(buzzerPin, 200, 300);
  delay(300);
  noTone(buzzerPin);
}
