// C++ code
//
String t="";
void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available())
  {
    char ta=Serial.read();
    t+=ta;
  }
  if(t=="amit is good"){
  digitalWrite(13,0);
    digitalWrite(12,1);
    digitalWrite(11,0);
    digitalWrite(10,1);
    
    
  }
  if(t=="off"){
  digitalWrite(13,0);
    digitalWrite(12,0);
    digitalWrite(11,0);
    digitalWrite(10,0);
    
  }
  if(t=="cc"){
  digitalWrite(13,0);
    digitalWrite(12,1);
    digitalWrite(11,0);
    digitalWrite(10,0);
    
  }
  if(t=="dd"){
  digitalWrite(13,0);
    digitalWrite(12,0);
    digitalWrite(11,0);
    digitalWrite(10,1);
    
  }
}

//bluetooth control driving car for two LED(MOTOR) and use a motor driver
//bluetooth control for one LED