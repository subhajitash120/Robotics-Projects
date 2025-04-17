#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3AFErITak"
#define BLYNK_TEMPLATE_NAME "Smart Fridge with Vital Monitoring Capabilities"
#define BLYNK_AUTH_TOKEN "ZX2xBT2WmVfKByXkoebHImYld6LZ7Uyk"

#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "SVU";
char pass[] = "12345678";

const float vCalibration = 41.5;
const float currCalibration = 0.15;

EnergyMonitor emon;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27 for 16x2 LCD

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);

  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

  delay(1000);
}

void loop() {
  Blynk.run();

  emon.calcVI(20, 2000);
  float voltage = emon.Vrms;
  float current = emon.Irms;
  float power = emon.apparentPower;

  Serial.printf("Vrms: %.2fV\tIrms: %.4fA\tPower: %.4fW\n", voltage, current, power);

  Blynk.virtualWrite(V0, voltage);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);

  lcd.setCursor(0, 0);  // First row
  lcd.print("V: ");
  lcd.print(voltage);
  lcd.print("V");

  lcd.setCursor(0, 1);  // Second row
  lcd.print("I: ");
  lcd.print(current);
  lcd.print("A ");
  lcd.print("P: ");
  lcd.print(power);
  lcd.print("W");

  delay(5000);
}
