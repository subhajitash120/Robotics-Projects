int led;
led = 6;
int slideSwitch; 
slideSwitch= 10;

void setup() {
pinMode(slideSwitch, INPUT);     // switch input pin
pinMode(led, OUTPUT);               //LED output pin
}
void loop() {
  if (digitalRead(slideSwitch) == HIGH)  //reading slide switch state
  {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
