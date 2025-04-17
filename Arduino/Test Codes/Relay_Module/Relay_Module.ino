// Arduino code to test a single-channel relay

const int relayPin = 12; // Change this to the pin you have connected the relay to

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Initially, turn off the relay
}

void loop() {
  // Turn on the relay for 2 seconds
  digitalWrite(relayPin, HIGH);
  delay(2000);

  // Turn off the relay for 2 seconds
  digitalWrite(relayPin, LOW);
  delay(2000);
}
