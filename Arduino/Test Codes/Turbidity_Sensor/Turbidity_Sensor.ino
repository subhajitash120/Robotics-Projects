const int turbidityPin = A0;

const float m = 0.125;
const float b = 0.0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int turbidityValue = analogRead(turbidityPin);
  
  float ntuValue = m * turbidityValue + b;
  
  Serial.print("Turbidity Value (ADC): ");
  Serial.print(turbidityValue);
  Serial.print(" | NTU: ");
  Serial.println(ntuValue);
  
  delay(1000);
}
