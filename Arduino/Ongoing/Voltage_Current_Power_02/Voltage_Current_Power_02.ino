#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3T1OZ_l3R"
#define BLYNK_TEMPLATE_NAME "Power Monitoring"
#define BLYNK_AUTH_TOKEN "vk8iUOE-EC6n2CLv9qn_c1j0yVcp6YYF"

#include "EmonLib.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <hd44780.h>  // Main hd44780 library
#include <hd44780ioClass/hd44780_I2Cexp.h>  // I2C expander i/o class

char ssid[] = "SVU";
char pass[] = "12345678";

const float vCalibration = 41.5;
const float currCalibration = 1.55;

EnergyMonitor emon;

hd44780_I2Cexp lcd;  // Declare lcd object using the I2Cexp class

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);

  lcd.begin(16, 2);  // Initialize the LCD as 16x2
  lcd.backlight();    // Turn on the backlight

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

  delay(1000);
}
