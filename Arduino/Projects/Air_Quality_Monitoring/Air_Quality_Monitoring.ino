#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL32BYAO_9k"
#define BLYNK_TEMPLATE_NAME "Air Quality Monitoring"
#define BLYNK_AUTH_TOKEN "VXONDaPC3T2O6oXVzsRyxcwNA-Rmy_wf"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "OPPO2";
char pass[] = "12345689";

int mq135Pin = A0;
float mq135Value;
float ppmValue;

const float offset = 200;
const float slope = 100;

bool notificationSent = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  
  mq135Value = analogRead(mq135Pin);
  ppmValue = pow(10, (mq135Value - offset) / slope);

  Blynk.virtualWrite(V0, mq135Value);
  Blynk.virtualWrite(V1, ppmValue);

  Serial.print("Analog Value: ");
  Serial.print(mq135Value);
  Serial.print(" | PPM Value: ");
  Serial.println(ppmValue);
  
  if (ppmValue > 500) {
    if (!notificationSent) {
      Blynk.logEvent("high_ppm_detected");
      notificationSent = true;
    }
  } else {
    notificationSent = false;
  }

  delay(1000);
}
