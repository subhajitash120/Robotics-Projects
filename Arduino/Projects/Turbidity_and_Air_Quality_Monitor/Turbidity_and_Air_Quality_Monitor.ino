#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3owAyTCrj"
#define BLYNK_TEMPLATE_NAME "Monitoring System"
#define BLYNK_AUTH_TOKEN "6QreQKr5VeLH0QdF0HmWtIS49UT4Z4Q7"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

char ssid[] = "Teja";
char pass[] = "Teja153624";

const int mq6Pin = 14;
const int turbidityPin = A0;
#define DHTPIN 13
#define DHTTYPE DHT11

const float m = 0.125;
const float b = 0.0;

DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int fontSize = 1;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  Blynk.run();

  int mq6Value = digitalRead(mq6Pin);
  int gasDetected = (mq6Value == HIGH) ? 0 : 1;

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  int turbidityValue = analogRead(turbidityPin);
  float ntuValue = m * turbidityValue + b;

  Blynk.virtualWrite(V0, gasDetected);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, ntuValue);

  Serial.print("Gas Detected: ");
  Serial.print(gasDetected);
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C | Humidity: ");
  Serial.print(humidity);
  Serial.print(" | Turbidity Value (ADC): ");
  Serial.print(turbidityValue);
  Serial.print(" | NTU: ");
  Serial.println(ntuValue);

  display.clearDisplay();
  display.setTextSize(fontSize);

  display.setCursor(0, 0);
  display.print("Gas: ");
  display.println(gasDetected ? "Yes" : "No");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

  display.setCursor(0, 48);
  display.print("Turbidity: ");
  display.print(ntuValue);
  display.println(" NTU");

  display.display();

  delay(1000);

}