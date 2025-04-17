// C++ code
//
int LED=13;

void setup()
{
  pinMode(LED, OUTPUT);
}

void loop()
{
  digitalWrite(LED, HIGH);
  delay(500); // Wait for 1000 millisecond(s)
  digitalWrite(LED,LOW);
  delay(500); // Wait for 1000 millisecond(s)
}