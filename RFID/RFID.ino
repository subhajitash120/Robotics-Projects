#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER 7

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to 0x3F if LCD doesn't work

byte allowedUID[] = {0x22, 0xD4, 0x70, 0x53}; // Replace with your card's UID

void setup() {
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
    
    lcd.init();
    lcd.backlight();
    
    pinMode(BUZZER, OUTPUT);
    
    lcd.setCursor(0, 0);
    lcd.print("Scan Your Card");
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return;
    }

    Serial.print("Card UID: ");
    bool accessGranted = true;
    
    for (byte i = 0; i < rfid.uid.size; i++) {
        Serial.print(rfid.uid.uidByte[i], HEX);
        if (rfid.uid.uidByte[i] != allowedUID[i]) {
            accessGranted = false;
        }
    }
    Serial.println();

    lcd.clear();
    lcd.setCursor(0, 0);

    if (accessGranted) {
        lcd.print("Access Allowed");
        digitalWrite(BUZZER, HIGH);
        delay(500);
        digitalWrite(BUZZER, LOW);
    } else {
        lcd.print("Access Denied");
        digitalWrite(BUZZER, HIGH);
        delay(1000);
        digitalWrite(BUZZER, LOW);
    }

    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan Your Card");

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
}
