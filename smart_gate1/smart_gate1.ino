#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define servo 10
#define trig 8
#define echo 9

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;

long duration;
int distance;

void setup()
{
  myServo.attach(servo);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome");
  delay(1000);
}

void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dist: ");
  lcd.print(distance);
  lcd.print(" cm");
  
  if (distance < 15) {
    myServo.write(90);
  } else {
    myServo.write(0);
  }
}