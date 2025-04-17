#define sensorPin A3
#define ledpin 13
void setup()
{
 pinMode(ledpin, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
  int rain = analogRead(sensorPin);
      Serial.print("rain level: ");
      Serial.println(rain); 
  
  if (rain <=500) {
        digitalWrite(ledpin, 1);
        Serial.println("rain started");
    } 
  else {
        digitalWrite(ledpin, 0);
        Serial.println("rain stop");
    }
  delay(3000);
}