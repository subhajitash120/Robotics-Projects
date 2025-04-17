#define sensorPin A3
#define buzzerPin 11
void setup()
{
 pinMode(buzzerPin, OUTPUT);
 Serial.begin(9600);
}

void loop()
{
  int moistureValue = analogRead(sensorPin);
      Serial.print("Soil Moisture Level: ");
      Serial.println(moistureValue); 
  
  if (moistureValue < 250) {
        analogWrite(buzzerPin, 100);
        Serial.println("Pump chalu kar bhai");
    } 
  else {
        analogWrite(buzzerPin, 0);
        Serial.println("pump bandh kar de, ho gaya");
    }
  delay(3000);
}