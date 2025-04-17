#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3AFErITak"
#define BLYNK_TEMPLATE_NAME "Smart Fridge with Vital Monitoring Capabilities"
#define BLYNK_AUTH_TOKEN "ZX2xBT2WmVfKByXkoebHImYld6LZ7Uyk"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "SVU";
char pass[] = "12345678";

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
}
