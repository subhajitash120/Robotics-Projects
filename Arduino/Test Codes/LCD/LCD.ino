#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address (the default address is 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight
  lcd.backlight();
  // Print "Hello, World!" on the LCD
  lcd.print("Hello, World!");
}

void loop() {
  // Nothing in the loop for this example
}
