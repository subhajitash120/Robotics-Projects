#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3F4ne6h2y"
#define BLYNK_TEMPLATE_NAME "Patient Monitoring and Control System"
#define BLYNK_AUTH_TOKEN "Y3yreWA4S0NK8sxxzlT-lsE6d9fubryw"

#include "src/OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define CAMERA_MODEL_AI_THINKER

#include "camera_pins.h"
#include <Arduino.h>

char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "SVU";
const char* password = "12345678";

const int lightRelayPin = 13;
const int doorLockRelayPin = 15;
const int buttonPin = 2;

int buttonState = 0;
int lastButtonState = 0;

OV2640 cam;
WebServer server(80);

const char HEADER[] = "HTTP/1.1 200 OK\r\n"
                      "Access-Control-Allow-Origin: *\r\n"
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);

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

  pinMode(lightRelayPin, OUTPUT);
  pinMode(doorLockRelayPin, OUTPUT);

  digitalWrite(lightRelayPin, LOW);
  digitalWrite(doorLockRelayPin, LOW);

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

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  pinMode(buttonPin, INPUT_PULLUP);

  cam.init(config);

  IPAddress ip;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  ip = WiFi.localIP();
  Serial.println(F("WiFi connected"));
  Serial.println("");

  Blynk.config(auth);
  Serial.println("Connecting to Blynk");
  while (Blynk.connect() == false) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Blynk");

  Serial.println(ip);
  Serial.print("Stream Link: http://");
  Serial.print(ip);
  Serial.println("/mjpeg/1");
  server.on("/mjpeg/1", HTTP_GET, handle_jpg_stream);
  server.on("/jpg", HTTP_GET, handle_jpg);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    Serial.println("Button pressed");
    Blynk.logEvent("patient_alert");
  }
  lastButtonState = buttonState;
  Blynk.run();
}

BLYNK_WRITE(V1) {
  int lightState = param.asInt();
  digitalWrite(lightRelayPin, lightState);
}

BLYNK_WRITE(V2) {
  int doorLockState = param.asInt();
  digitalWrite(doorLockRelayPin, doorLockState);
}
