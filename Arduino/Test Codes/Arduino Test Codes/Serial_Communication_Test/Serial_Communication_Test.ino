void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char receivedChar = Serial.read();
    Serial.print("Received: ");
    Serial.println(receivedChar);
    
    // Echo the received character back to the computer
    Serial.print("Echo: ");
    Serial.println(receivedChar);
  }
}
