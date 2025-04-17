const int reedPin = 2;    // Pin where the reed switch is connected

void setup() {
  pinMode(reedPin, INPUT);      // Set the reed switch pin as input
  Serial.begin(9600);           // Initialize serial communication for debugging
}

void loop() {
  int reedState = digitalRead(reedPin);  // Read the state of the reed switch

  if (reedState == HIGH) {
    Serial.println("Magnet detected!");
  } else {
    Serial.println("No magnet.");
  }

  delay(100);  // Small delay to debounce the switch
}
