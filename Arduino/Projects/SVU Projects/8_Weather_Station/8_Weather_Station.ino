#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3eKUYSVWU"
#define BLYNK_TEMPLATE_NAME "Weather Monitoring Station"
#define BLYNK_AUTH_TOKEN "McrnjqpjZzl2pCgaVnN2S9x4XIO7EZwN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

char ssid[] = "SVU";
char pass[] = "12345678";

Adafruit_BME280 bme;
unsigned long delayTime;

BlynkTimer timer;

#define SEALEVELPRESSURE_HPA (1013.25)

void sendSensorData() {
  float temperature = bme.readTemperature();
  float pressure = bme.readPressure() / 100.0F;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.readHumidity();
  float windSpeed = sqrt(pressure / 0.00256);
  float airDensity = pressure * 100 / (8.314 * (temperature + 273.15));

  Blynk.virtualWrite(V1, pressure);
  Blynk.virtualWrite(V2, altitude);
  Blynk.virtualWrite(V3, humidity);
  Blynk.virtualWrite(V4, windSpeed);
  Blynk.virtualWrite(V0, airDensity);

  if (temperature < 20) {
    Blynk.logEvent("temperature_alert");
  } else if (temperature > 40) {
    Blynk.logEvent("temperature_alert");
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.begin(115200);
  Serial.println(F("BME280 test"));

  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;
  Serial.println();

  timer.setInterval(10000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
