#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3Yigo05eZ"
#define BLYNK_TEMPLATE_NAME "Fan Speed"
#define BLYNK_AUTH_TOKEN "fnp8LRidzRZwFEQkxV1NQanzofNwVEBO"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "mvsk";
char pass[] = "9441762578";

#define DHTPIN 4     
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#define IN1 14  
#define IN2 12  
#define ENA 0   

BlynkTimer timer;

int fanSpeed = 0;

void controlFan() {
  if (fanSpeed > 0) {  
    digitalWrite(IN1, HIGH);  
    digitalWrite(IN2, LOW);   
    analogWrite(ENA, fanSpeed);  
  } else {
    digitalWrite(IN1, LOW);  
    digitalWrite(IN2, LOW);  
    analogWrite(ENA, 0);  
  }
}

BLYNK_WRITE(V2) {
  fanSpeed = param.asInt();
}

void sendSensorData() {
  float temperature = dht.readTemperature(); 
  float humidity = dht.readHumidity();       

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Blynk.virtualWrite(V0, temperature);
  Blynk.virtualWrite(V1, humidity);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);

  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
  controlFan();
}
