int pirPin = 2;  // PIR sensor output pin
int ledPin = 13; // LED pin

void setup() {
    pinMode(pirPin, INPUT);
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
}

void loop() {
    int motion = digitalRead(pirPin); // Read PIR sensor
    if (motion == HIGH) {  // If motion is detected
        digitalWrite(ledPin, HIGH);  // Turn on LED
        Serial.println("Motion detected!");
        delay(10);  // Wait for a second
    } else {
        digitalWrite(ledPin, LOW);  // Turn off LED
    }
}
