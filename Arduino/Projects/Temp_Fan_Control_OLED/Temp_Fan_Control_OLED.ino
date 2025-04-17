#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ONE_WIRE_BUS 2
#define FAN_PIN 9
#define TEMP_MIN 20
#define TEMP_MAX 33

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int fontSize = 2;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library with Fan Control");

  sensors.begin();
  pinMode(FAN_PIN, OUTPUT);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(fontSize);
}

void loop(void)
{ 
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);

    int fanSpeed = 0;
    if (tempC >= TEMP_MIN && tempC <= TEMP_MAX)
    {
      fanSpeed = map(tempC, TEMP_MIN, TEMP_MAX, 0, 255);
    }
    else if (tempC > TEMP_MAX)
    {
      fanSpeed = 255;
    }
    else
    {
      fanSpeed = 0;
    }

    analogWrite(FAN_PIN, fanSpeed);

    int fanSpeedPercentage = map(fanSpeed, 0, 255, 0, 100);

    Serial.print("Fan speed (PWM): ");
    Serial.println(fanSpeed);
    Serial.print("Fan speed percentage: ");
    Serial.println(fanSpeedPercentage);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temp:");
    display.print(tempC);
    display.print("C");

    display.setCursor(0, 40);
    display.print("Fan: ");
    display.print(fanSpeedPercentage);
    display.print("%");

    display.display();
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

  delay(1000);
}
