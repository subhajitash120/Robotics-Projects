#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3T1OZ_l3R"
#define BLYNK_TEMPLATE_NAME "Power Monitoring"
#define BLYNK_AUTH_TOKEN "vk8iUOE-EC6n2CLv9qn_c1j0yVcp6YYF"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

const int voltagePin = 34;
#define currentpin 4

float R1 = 6800.0;
float R2 = 12000.0;

float voltageCalibrationFactor = 220.0 / 4095.0;

char ssid[] = "SVU";
char pass[] = "12345678";

hd44780_I2Cexp lcd(0x27);  // I2C address of the LCD

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  pinMode(voltagePin, INPUT);
  pinMode(currentpin, INPUT);

  lcd.begin(16, 2);
  lcd.setBacklight(255);
  lcd.print("Power Monitor");
}

void loop() {
  Blynk.run();

  int voltageRawValue = analogRead(voltagePin);
  float voltageMeasured = voltageRawValue * voltageCalibrationFactor;
  
  int adc = analogRead(currentpin);
  float adc_voltage = adc * (3.3 / 4096.0);
  float current_voltage = (adc_voltage * (R1 + R2) / R2);
  float current = (current_voltage - 2.5) / 0.100;

  float power = voltageMeasured * current;

  Serial.print("Voltage Value: ");
  Serial.print(voltageMeasured, 2);
  Serial.println(" V");

  Serial.print("Current Value: ");
  Serial.print(current, 2);
  Serial.println(" A");

  Serial.print("Power Value: ");
  Serial.print(power, 2);
  Serial.println(" W");

  Blynk.virtualWrite(V0, voltageMeasured);
  Blynk.virtualWrite(V1, current);
  Blynk.virtualWrite(V2, power);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(voltageMeasured, 2);
  lcd.print("V");

  lcd.setCursor(0, 1);
  lcd.print("I: ");
  lcd.print(current, 2);
  lcd.print("A P: ");
  lcd.print(power, 2);
  lcd.print("W");

  delay(1000);
}
