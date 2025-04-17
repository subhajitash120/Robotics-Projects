#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL39fZRqyDZ"
#define BLYNK_TEMPLATE_NAME "Car Safety"
#define BLYNK_AUTH_TOKEN "_oXT0KRVx0sIgFO_3ho6H4nDQQ-HAjGx"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NewPing.h>

#define TRIGGER_PIN 14   // NodeMCU pin for ultrasonic sensor trigger
#define ECHO_PIN 12      // NodeMCU pin for ultrasonic sensor echo
#define IR_PIN 4         // Digital pin for IR sensor
#define MQ_DIGITAL_PIN 5 // Digital pin for MQ-5 sensor
#define MOTOR_PIN 0      // Digital pin for controlling the motor
#define BUZZER_PIN 13    // Digital pin for the buzzer

#define MAX_DISTANCE 200 // Maximum distance in centimeters

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

char ssid[] = "charan";
char pass[] = "Charan1804@";

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(MQ_DIGITAL_PIN, INPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  delay(500); // Delay for stability

  // Read sensors
  int irValue = digitalRead(IR_PIN);
  int mqValue = digitalRead(MQ_DIGITAL_PIN);
  unsigned int distance = sonar.ping_cm();

  // Send sensor data to Blynk
  Blynk.virtualWrite(V1, irValue);
  Blynk.virtualWrite(V2, !mqValue);
  Blynk.virtualWrite(V0, distance);

  // Check conditions
  if (irValue == LOW && mqValue == HIGH && distance >= 20) {
    // Motor should rotate at full speed (255)
    analogWrite(MOTOR_PIN, 255);
    Serial.println("Full Speed");
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer if it was previously on
  } else if (irValue == LOW && mqValue == HIGH && distance < 20) {
    // Motor should rotate at half speed (150)
    analogWrite(MOTOR_PIN, 100);
    Serial.println("Obstacle Reducing Speed!");
    digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer if it was previously on
  } else {
    // Vehicle Start Aborted condition
    analogWrite(MOTOR_PIN, 0);
    Serial.println("Vehicle Start Aborted!");
    digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
    Blynk.logEvent("driver_condition");
  }

  delay(1000); // Delay between readings
}
