#define TRIG_PIN 9  // Trigger pin of ultrasonic sensor
#define ECHO_PIN 10 // Echo pin of ultrasonic sensor
#define LED_PIN 13   // LED pin

const int thresholdDistance = 10; // Distance in cm to turn LED on

void setup() {
    Serial.begin(9600); // Start Serial Monitor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // Send ultrasonic pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the echo time
    long duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.0343 / 2; // Convert time to distance in cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Turn LED ON if object is closer than threshold
    if (distance > 0 && distance <= thresholdDistance) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON - Object Detected!");
    } else {
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF - No Object");
    }

    delay(500); // Wait before next measurement
}
