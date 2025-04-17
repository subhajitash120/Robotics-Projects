// C++ code
//
int IR=7;
// int LED=9;
int buzz= 13;
void setup()
{
  pinMode(IR, INPUT);
  // pinMode(LED, OUTPUT);
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  int sensor= digitalRead(IR);
  if(sensor==LOW){
    // digitalWrite(LED,HIGH);
    digitalWrite(buzz,HIGH);
    Serial.println("Obstacle detected");
  }
  else{
    // digitalWrite(LED,LOW);
    digitalWrite(buzz,LOW);
    Serial.println("Obstacle not detected");
  }
 delay(100);
  
}