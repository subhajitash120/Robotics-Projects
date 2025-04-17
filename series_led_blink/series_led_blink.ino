int leds[] = {2, 3, 4, 5, 6}; // Array to store LED pins
const int numLeds = sizeof(leds) / sizeof(leds[0]);

void setup() {
  for (int i = 0; i < numLeds; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  // Forward sequence
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(leds[i], HIGH);
    delay(1000);
    digitalWrite(leds[i], LOW);
    delay(1000);
  }
  
  // Reverse sequence
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(leds[i], HIGH);
    delay(1000);
    digitalWrite(leds[i], LOW);
    delay(1000);
  }
}
