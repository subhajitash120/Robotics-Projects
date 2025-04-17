#define BLYNK_PRINT Serial

int LED_PIN = 16; // Use D0 for the LED, connected to GPIO 16

#define BLYNK_TEMPLATE_ID "TMPL3PlhaeVJu"     // Identifier for the Blynk template
#define BLYNK_TEMPLATE_NAME "Template 1"       // Name of the Blynk template
#define BLYNK_AUTH_TOKEN "t5KI_6cEsDaz78Nz6-Ptd2jsSqW2d6Yy"  // Authorization token for Blynk

#include <ESP8266WiFi.h>          // Include the ESP8266WiFi library for ESP8266 Wi-Fi functionality
#include <BlynkSimpleEsp8266.h>   // Include the Blynk library for ESP8266

char ssid[] = "Cookie";    // WiFi network SSID
char pass[] = "123456780"; // WiFi network password

void setup()
{
  Serial.begin(9600);       // Initialize serial communication at 9600 baud rate
  pinMode(LED_PIN, OUTPUT); // Set the LED pin (D0 or GPIO 16) as an output
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);  // Initialize Blynk with the given authorization token and Wi-Fi credentials
}

void loop()
{
  Blynk.run();  // Continuously run Blynk to maintain the connection
}

// Blynk virtual pin V0 write event handler
BLYNK_WRITE(V0) {
  int pinValue = param.asInt();  // Read the value sent to virtual pin V0
  if (pinValue == 1) {
    digitalWrite(LED_PIN, HIGH); // Turn LED on if the received value is 1
  } else {
    digitalWrite(LED_PIN, LOW); // Turn LED off if the received value is not 1
  }
}
