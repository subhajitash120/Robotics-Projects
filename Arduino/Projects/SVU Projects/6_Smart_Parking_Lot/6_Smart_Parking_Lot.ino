#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3Yi_qqSD5"
#define BLYNK_TEMPLATE_NAME "Smart Parking Lot"
#define BLYNK_AUTH_TOKEN "NmJ70cRWu5WczYTD-oWgD-r2NZvELp1d"

#include "src/OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NewPing.h>
#include <ESP32Servo.h>  // Use the ESP32Servo library

#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"

// Blynk authentication and WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "SVU";
const char* password = "12345678";

// Camera setup
OV2640 cam;
WebServer server(80);

// HTTP response headers for camera streaming
const char HEADER[] = "HTTP/1.1 200 OK\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);

// Reed switch pins
const int reedPin1 = 12;
const int reedPin2 = 3;

// Ultrasonic sensor pins and settings
#define TRIGGER_PIN 14
#define ECHO_PIN 15
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Servo setup
Servo myservo;  // Use the ESP32Servo class
const int servoPin = 2;

// LED pin
const int ledPin = 4;

// Blynk virtual pin setup
BLYNK_WRITE(V0) {
  int servoAngle = param.asInt();
  myservo.write(servoAngle);
}

BLYNK_WRITE(V3) {
  int ledState = param.asInt();
  digitalWrite(ledPin, ledState);
}

void handle_jpg_stream(void) {
  char buf[32];
  int s;

  WiFiClient client = server.client();

  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);

  while (true) {
    if (!client.connected()) break;
    cam.run();
    s = cam.getSize();
    client.write(CTNTTYPE, cntLen);
    sprintf(buf, "%d\r\n\r\n", s);
    client.write(buf, strlen(buf));
    client.write((char*)cam.getfb(), s);
    client.write(BOUNDARY, bdrLen);
  }
}

const char JHEADER[] = "HTTP/1.1 200 OK\r\n"
                       "Content-disposition: inline; filename=capture.jpg\r\n"
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

void handle_jpg(void) {
  WiFiClient client = server.client();

  cam.run();
  if (!client.connected()) return;

  client.write(JHEADER, jhdLen);
  client.write((char*)cam.getfb(), cam.getSize());
}

void handleNotFound() {
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text/plain", message);
}

void setup() {
  Serial.begin(115200);

  // Camera configuration
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  cam.init(config);

  // WiFi setup
  IPAddress ip;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.println(F("WiFi connected"));
  Serial.println(ip);

  // Blynk setup
  Blynk.config(auth);
  Serial.println("Connecting to Blynk");
  while (Blynk.connect() == false) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Blynk");

  // HTTP server setup for camera streaming
  Serial.print("Stream Link: http://");
  Serial.print(ip);
  Serial.println("/mjpeg/1");
  server.on("/mjpeg/1", HTTP_GET, handle_jpg_stream);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.onNotFound(handleNotFound);
  server.begin();

  // Reed switch setup
  pinMode(reedPin1, INPUT);
  pinMode(reedPin2, INPUT);

  // Servo setup
  myservo.attach(servoPin);

  // LED setup
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Handle HTTP server client
  server.handleClient();

  // Handle Blynk
  Blynk.run();

  // Reed switch state
  int reedState1 = digitalRead(reedPin1);
  int reedState2 = digitalRead(reedPin2);

  // Send reed switch state to Blynk
  Blynk.virtualWrite(V1, reedState1 == HIGH ? HIGH : LOW);
  Blynk.virtualWrite(V2, reedState2 == HIGH ? HIGH : LOW);

  // Ultrasonic sensor measurement
  delay(500);                               // Delay for stability
  unsigned int distance = sonar.ping_cm();  // Get distance in centimeters

  if (distance == 0) {
    Serial.println("Error: No echo received!");
  } else {
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Control servo based on distance
    if (distance < 20) {
      myservo.write(90);  // Move servo to 180 degrees
      Blynk.virtualWrite(V0, 90);
    } else {
      myservo.write(0);  // Move servo to 0 degrees
      Blynk.virtualWrite(V0, 0);
    }
  }

  delay(100);  // Small delay to debounce the switch and allow for stable sensor reading
}
