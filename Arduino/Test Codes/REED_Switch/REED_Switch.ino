const int REED_PIN_1 = 18; // Analog pin connected to reed switch 1
const int REED_PIN_2 = 5; // Analog pin connected to reed switch 2
const int LED_PIN = 26;    // LED pin

void setup() {
  Serial.begin(9600);
  pinMode(REED_PIN_1, INPUT_PULLUP); // Enable internal pull-up for reed switch 1
  pinMode(REED_PIN_2, INPUT_PULLUP); // Enable internal pull-up for reed switch 2
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int proximity1 = digitalRead(REED_PIN_1); // Read the state of reed switch 1
  int proximity2 = digitalRead(REED_PIN_2); // Read the state of reed switch 2

  // If the first switch is closed, turn on the LED and print a message
  if (proximity1 == LOW) {
    Serial.println("Switch 1 closed");
    digitalWrite(LED_PIN, HIGH); // Turn the LED on
  }
  // If the second switch is closed, turn on the LED and print a message
  else if (proximity2 == LOW) {
    Serial.println("Switch 2 closed");
    digitalWrite(LED_PIN, HIGH); // Turn the LED on
  } else {
    Serial.println("Both switches opened");
    digitalWrite(LED_PIN, LOW); // Turn the LED off
  }
}
