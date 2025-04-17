#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS3231 rtc;

const char* monthNames[] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int BuzzerPin = 10;
bool userInputMode = false;
int targetHour = 0;
int targetMinute = 0;
int userInputCount = 0;

String formatDigits(int digits);

void setup() {
  Serial.begin(9600);

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }
  display.clearDisplay();

  // Initialize the RTC module
  if (!rtc.begin()) {
    while (1);
  }

  // Set the RTC to the date & time of the compilation if RTC lost power
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Set BuzzerPin as an OUTPUT
  pinMode(BuzzerPin, OUTPUT);
}

void loop() {
  DateTime now = rtc.now();
  display.clearDisplay();

  if (!userInputMode) {
    // Display current time and date on OLED
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Time: ");
    display.print(formatDigits(now.hour()));
    display.print(':');
    display.print(formatDigits(now.minute()));
    display.print(':');
    display.println(formatDigits(now.second()));

    display.print("Date: ");
    display.print(now.day(), DEC);
    display.print(' ');
    display.print(monthNames[now.month()]);
    display.print(' ');
    display.println(now.year(), DEC);

    // Check if it's time to activate the buzzer
    if (now.hour() == targetHour && now.minute() == targetMinute) {
      display.println("Bell Status: ON");
      digitalWrite(BuzzerPin, HIGH);
    } else {
      display.println("Bell Status: OFF");
      digitalWrite(BuzzerPin, LOW);
    }

    display.println("User Mode Press '#'");
  } else {
    handleUserInputConfirmation();
  }

  display.display();

  char key = keypad.getKey();

  if (userInputMode) {
    handleUserInput(key);
  }

  // Enter user input mode when '#' key is pressed
  if (key == '#' && !userInputMode) {
    Serial.print("Set Time to Ring the Bell: ");
    userInputMode = true;
  }

  // Activate buzzer if it's the specified time
  if (now.hour() == targetHour && now.minute() == targetMinute) {
    digitalWrite(BuzzerPin, HIGH);
  } else {
    digitalWrite(BuzzerPin, LOW);
  }
}

void handleUserInput(char key) {
  if (key >= '0' && key <= '9') {
    // Process user input for setting the target time
    if (userInputCount == 0) {
      targetHour = (key - '0') * 10;
      userInputCount++;
    } else if (userInputCount == 1) {
      targetHour += (key - '0');
      if (targetHour > 23) {
        targetHour = 23;
      }
      userInputCount++;
    } else if (userInputCount == 2) {
      targetMinute = (key - '0') * 10;
      userInputCount++;
    } else if (userInputCount == 3) {
      targetMinute += (key - '0');
      if (targetMinute > 59) {
        targetMinute = 59;
      }
      userInputMode = false;
      userInputCount = 0;

      // Display user input confirmation message
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.print("User Input Complete");
      display.display();

      delay(2000);
      display.clearDisplay();
    }
  } else if (key == 'C') {
    // Cancel user input and reset variables
    userInputMode = false;
    userInputCount = 0;
    targetHour = 0;
    targetMinute = 0;

    // Display user input cancellation message
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("User Input Canceled");
    display.display();

    delay(2000);
    display.clearDisplay();
  } 
}

void handleUserInputConfirmation() {
  // Display the confirmed target time during user input mode
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Set Time to Ring the Bell: ");
  display.print(formatDigits(targetHour));
  display.print(':');
  display.print(formatDigits(targetMinute));
  display.println();
}

String formatDigits(int digits) {
  return (digits < 10) ? ("0" + String(digits)) : String(digits);
}
