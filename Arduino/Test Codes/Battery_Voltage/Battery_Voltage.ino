#include <ESP8266WiFi.h>

int analogInPin = A0;
int sensorValue;
float calibration = 0.21;
int bat_percentage;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(analogInPin);
  float voltage = (((sensorValue * 3.3) / 1024) * 2 + calibration);

  bat_percentage = mapfloat(voltage, 2.8, 4.2, 0, 100);

  if (bat_percentage >= 100) {
    bat_percentage = 100;
  } else if (bat_percentage <= 0) {
    bat_percentage = 1;
  }

  Serial.print("Analog Value = ");
  Serial.print(sensorValue);
  Serial.print("\t Output Voltage = ");
  Serial.print(voltage);
  Serial.print("\t Battery Percentage = ");
  Serial.println(bat_percentage);
  delay(1000);
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}