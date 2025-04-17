#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3d0_y0lVf"
#define BLYNK_TEMPLATE_NAME "Fish Feeding System"
#define BLYNK_AUTH_TOKEN "SZ8enGm4o46dmtlSfhLB2M4l-Ggq3LxF"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char ssid[] = "Y_5";
char pass[] = "Mugiwara";

Servo myServo;
int servoPin = 0;
int waterLevelPin = A0;
int lightPin = 5;

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(0);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  sensors.begin();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (!sensors.getAddress(sensor1, 0)) Serial.println("Unable to find address for Device 0");
  Serial.print("Device 0 Address: ");
  printAddress(sensor1);
  Serial.println();
  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, LOW);
}

BLYNK_WRITE(V0) {
  int sliderValue = param.asInt();
  int servoPosition;
  switch (sliderValue) {
    case 0:
      servoPosition = 0;
      break;
    case 1:
      servoPosition = 30;
      break;
    case 2:
      servoPosition = 80;
      break;
    case 3:
      servoPosition = 130;
      break;
    case 4:
      servoPosition = 180;
      break;
    default:
      servoPosition = 0;
  }
  myServo.write(servoPosition);
  Serial.print("Slider value: ");
  Serial.print(sliderValue);
  Serial.print(" - Servo position: ");
  Serial.println(servoPosition);
}

BLYNK_WRITE(V3) {
  int lightState = param.asInt();
  if (lightState == 1) {
    digitalWrite(lightPin, HIGH);
    Serial.println("Light turned ON");
  } else {
    digitalWrite(lightPin, LOW);
    Serial.println("Light turned OFF");
  }
}

void sendWaterTemperature() {
  sensors.requestTemperatures();
  float waterTempC = sensors.getTempC(sensor1);
  Serial.print("Water Temperature: ");
  Serial.print(waterTempC);
  Serial.println("°C");
  Blynk.virtualWrite(V1, waterTempC);
}

void sendWaterLevel() {
  int waterLevelValue = analogRead(waterLevelPin);
  Serial.print("Water Level: ");
  Serial.println(waterLevelValue);
  Blynk.virtualWrite(V2, waterLevelValue);
  String color;
  if (waterLevelValue < 200) {
    color = "#FF0000";
  } else if (waterLevelValue < 400) {
    color = "#0000FF";
  } else {
    color = "#00FF00";
  }
  Blynk.setProperty(V2, "color", color);
}

void loop() {
  Blynk.run();
  sendWaterTemperature();
  sendWaterLevel();
  delay(1000);
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
