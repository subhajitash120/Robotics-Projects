#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM800L(7, 8); // RX, TX
const int pinNumber = 5; // Pin number you want to control
const String phoneNumber = "+919700104930"; // Change to the desired phone number

bool motorState = false; // Variable to track the state of the motor

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32  // Change the height to 32 for a 128x32 display
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(pinNumber, OUTPUT); // Set pin 2 as an output

  Serial.begin(9600); // Start serial communication for debugging
  SIM800L.begin(9600); // Setting the baud rate of SIM800L
  delay(1000);
  SIM800L.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(1000);
  SIM800L.println("AT+CNMI=2,2,0,0,0"); // Configuring SMS notifications
  delay(1000);

  // initialize with the I2C addr 0x3C (for the 128x32)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Initial OLED display update
  updateOLED();

  // Print "System ready" to Serial Monitor
  Serial.println("System ready");

  // Send initial SMS
  sendSMS("System is ready");
}

void loop() {
  if (SIM800L.available()) {
    String message = SIM800L.readString();

    if (message.indexOf("Turn ON") >= 0) {
      if (!motorState) {
        digitalWrite(pinNumber, HIGH); // Turn on the motor
        sendSMS("Motor turned ON successfully");
        motorState = true;
      }
    } else if (message.indexOf("Turn OFF") >= 0) {
      if (motorState) {
        digitalWrite(pinNumber, LOW); // Turn off the motor
        sendSMS("Motor turned OFF successfully");
        motorState = false;
      }
    }

    // Print motor status to Serial Monitor
    Serial.print("Motor Status: ");
    Serial.println(motorState ? "ON" : "OFF");

    // Update OLED display with current motor status
    updateOLED();
  }
}

void updateOLED() {
  // Clear the buffer
  display.clearDisplay();
  
  // Set text size, color, and print the motor status
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Motor Status: ");
  display.println(motorState ? "ON" : "OFF");
  
  // Display the message
  display.display();
}

void sendSMS(String text) {
  String smsCommand = "AT+CMGS=\"" + phoneNumber + "\"";
  SIM800L.println(smsCommand); // Change to the phone number you want to send to
  delay(1000);
  SIM800L.println(text); // The SMS text you want to send
  delay(1000);
  SIM800L.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}