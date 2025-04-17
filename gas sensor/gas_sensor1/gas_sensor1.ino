const int gasSensorPin = A3;
const int ledPin = 13;
const int threshold = 200;
void setup()
{
    Serial.begin(9600);
    pinMode(gasSensorPin, INPUT);
    pinMode(ledPin, OUTPUT);
}

void loop()
{
    int gasValue = analogRead(gasSensorPin);
    Serial.print("Gas Level: ");
    Serial.println(gasValue);
  
        if(gasValue > threshold ) {
        digitalWrite(ledPin, HIGH);
        Serial.println("Warning! Gas Detected!");
    } 
        else{
        digitalWrite(ledPin, LOW);
    }
  delay(1000);
}