// C++ code
//
void setup()
{ 
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(12, INPUT);
  pinMode(11, OUTPUT);
}

void loop()
{
    long duration;
    int distance;
    
    // Send a short pulse to trigger pin
    digitalWrite(11, LOW);
    delay(2);
    digitalWrite(11, HIGH);
    delay(10);
    digitalWrite(11, LOW);

    // Measure the time it takes for the echo to return
    duration = pulseIn(12, HIGH);
    
    // Convert the time into distance (in cm)
    distance = duration * 0.034 / 2;
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  
  if(distance>=50){
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  
    Serial.println("Motors run");
  }
  else if(distance<50){
    digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
    
    Serial.println("Motors stopped");
  }
  delay(2000);
}