#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 'images (1)', 128x64px
const unsigned char epd_bitmap_images__1_ [] PROGMEM = {
   //Bitmap Image Code
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 1040)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	epd_bitmap_images__1_
};


void setup() {
    // Initialize the OLED display using Wire library
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop indefinitely
    }

    // Clear the display buffer
    display.clearDisplay();

    // Draw the bitmap
    display.drawBitmap(0, 0, epd_bitmap_images__1_, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);

    // Display the updated buffer content
    display.display();

    // Wait for a moment to see the display
    delay(5000); // Pause for 5 seconds
}

void loop() {
    // This is an optional loop function.
    // You can add any additional functionality here if needed.
}
