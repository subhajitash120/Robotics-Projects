#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3Sb9g9Tk1"
#define BLYNK_TEMPLATE_NAME "Plant Monitoring system"
#define BLYNK_AUTH_TOKEN "PGxVWJyjFGpkLxbVUeF7R9bIu3xU5_iX"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

char ssid[] = "Cookie";
char pass[] = "123456780";

#define DHTPIN 13
#define DHTTYPE DHT11
#define soilMoisturePin 15

const int buttonPin = 10;
const int pirSensorPin = 14;
const int relayPin = 12;

DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

int relayState = LOW;
int prevButtonState = HIGH;
int blynkButtonState = LOW;

void setup() {
  Serial.begin(9600);

  lcd.init(); lcd.backlight();
  lcd.print("Plant Monitoring"); lcd.setCursor(0, 1);
  lcd.print("System");

  dht.begin();

  pinMode(soilMoisturePin, INPUT);
  pinMode(pirSensorPin, INPUT);
  pinMode(relayPin, OUTPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(buttonPin, INPUT_PULLUP);

  Blynk.syncVirtual(V4);
}

BLYNK_WRITE(V4) {
  blynkButtonState = param.asInt();
}

void loop() {
  Blynk.run();

  float temperature = dht.readTemperature();
  int soilMoisture = digitalRead(soilMoisturePin);
  int pirState = digitalRead(pirSensorPin);
  soilMoisture = (soilMoisture == HIGH) ? LOW : HIGH;
  int buttonState = digitalRead(buttonPin);

  relayState = digitalRead(relayPin);

 // Check if soil moisture is low, Blynk button is high, or button is pressed
  if (soilMoisture == LOW || blynkButtonState == HIGH || buttonState == LOW ) {
    // Turn on the relay
    digitalWrite(relayPin, HIGH);
    relayState = HIGH;
  } else {
    // Turn off the relay
    digitalWrite(relayPin, LOW);
    relayState = LOW;
  }


  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TE: ");
  lcd.print((int)temperature);
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.print("WA: ");
  lcd.print(soilMoisture == HIGH ? "Wet" : "Dry");

  lcd.setCursor(0, 1);
  lcd.print("AN: ");
  lcd.print(pirState == HIGH ? "Yes" : "No");
  lcd.setCursor(8, 1);
  lcd.print("MO: ");
  lcd.print(relayState == HIGH ? "On" : "Off");

  if (isnan(temperature))
    Serial.println("Failed to read from DHT sensor!");

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, soilMoisture);
  Blynk.virtualWrite(V2, pirState);
  Blynk.virtualWrite(V3, relayState);
}
