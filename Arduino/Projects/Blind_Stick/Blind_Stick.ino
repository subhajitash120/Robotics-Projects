#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3JS-G1uKh"
#define BLYNK_TEMPLATE_NAME "Blind Stick"
#define BLYNK_AUTH_TOKEN "emlM1trsbqvDjlaXUysHQSWetDOeFCQE"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

char ssid[] = "vivoT2x";
char pass[] = "123456789";

#define TRIGGER_PIN 14
#define ECHO_PIN 12
#define MAX_DISTANCE 200

#define WATER_LEVEL_PIN 13
#define BUZZER_PIN 15
#define VIBRATOR_PIN 3

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Ultrasonic Sensor Initialized!");

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }

  Serial.println("");
  delay(100);

  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATOR_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(VIBRATOR_PIN, LOW);

  Serial.println("Water Level Sensor Initialized!");
}

void loop() {
  Blynk.run();

  unsigned int distance = sonar.ping_cm();

  Serial.println("Sensor Readings:");

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print("Acceleration - X: ");
  Serial.print(a.acceleration.x);
  Serial.print(" m/s^2, Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(" m/s^2, Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  if (distance == 0) {
    Serial.println("Distance: Error - No echo received!");
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATOR_PIN, LOW);
  } else if (distance > 0 && distance < 30) {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(VIBRATOR_PIN, HIGH);
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATOR_PIN, LOW);
  }

  int waterLevelState = digitalRead(WATER_LEVEL_PIN);
  if (waterLevelState == HIGH) {
    Serial.println("Water Level: YES");
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(VIBRATOR_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATOR_PIN, LOW);
    Blynk.logEvent("water_detected");
  } else {
    Serial.println("Water Level: NO");
  }

  if (abs(a.acceleration.x) > 5 || abs(a.acceleration.x) < -5) {
    Blynk.logEvent("fall_detected");
  }

  if (abs(a.acceleration.y) > -5) {
    Blynk.logEvent("fall_detected");
  }

  Serial.println("");

  Blynk.virtualWrite(V1, temp.temperature);
  Blynk.virtualWrite(V0, distance);

  delay(500);
}
