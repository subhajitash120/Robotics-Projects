                       #define ENA 10      
#define IN1 9       
#define IN2 8       
#define IN3 7       
#define IN4 6       
#define ENB 5       
#define IrRight A0  
#define IrFront A1  
#define IrLeft A2   
#define myServo 11  
#define pump 3     

int Speed = 160;                              
int PumpSpeed = 155;                          
int irRightValue, irFrontValue, irLeftValue;  

void setup() {
  Serial.begin(9600);        
  pinMode(IrRight, INPUT);   
  pinMode(IrFront, INPUT);   
  pinMode(IrLeft, INPUT);    
  pinMode(ENA, OUTPUT);      
  pinMode(IN1, OUTPUT);      
  pinMode(IN2, OUTPUT);      
  pinMode(IN3, OUTPUT);      
  pinMode(IN4, OUTPUT);      
  pinMode(ENB, OUTPUT);      
  pinMode(myServo, OUTPUT);  
  pinMode(pump, OUTPUT);     

  for (int angle = 90; angle <= 140; angle += 5) {  
    servoPulse(myServo, angle);
  }
  for (int angle = 140; angle >= 40; angle -= 5) {  
    servoPulse(myServo, angle);
  }
  for (int angle = 40; angle <= 95; angle += 5) {  
    servoPulse(myServo, angle);
  }

  analogWrite(ENA, Speed);  
  analogWrite(ENB, Speed);  
  delay(500);               
}

void loop() {
  irRightValue = map(analogRead(IrRight), 0, 1023, 1023, 0);
  irFrontValue = map(analogRead(IrFront), 0, 1023, 1023, 0);
  irLeftValue = map(analogRead(IrLeft), 0, 1023, 1023, 0);

  Serial.print(irRightValue);
  Serial.print("\t");
  Serial.print(irFrontValue);
  Serial.print("\t");
  Serial.println(irLeftValue);
  delay(50);

  if (irFrontValue < 250) {  
    Stop();
    analogWrite(pump, PumpSpeed);                     
    for (int angle = 90; angle <= 140; angle += 3) {  
      servoPulse(myServo, angle);
    }
    for (int angle = 140; angle >= 40; angle -= 3) {  
      servoPulse(myServo, angle);
    }
    for (int angle = 40; angle <= 90; angle += 3) {  
      servoPulse(myServo, angle);
    }
  } else if (irRightValue < 250) {  
    Stop();
    analogWrite(pump, PumpSpeed);                    
    for (int angle = 90; angle >= 40; angle -= 3) {  
      servoPulse(myServo, angle);
    }
    for (int angle = 40; angle <= 90; angle += 3) {  
      servoPulse(myServo, angle);
    }
  } else if (irLeftValue < 250) {  
    Stop();
    analogWrite(pump, PumpSpeed);                     
    for (int angle = 90; angle <= 140; angle += 3) {  
      servoPulse(myServo, angle);
    }
    for (int angle = 140; angle >= 90; angle -= 3) {  
      servoPulse(myServo, angle);
    }
  } else if (irRightValue >= 251 && irRightValue <= 700) {  
    analogWrite(pump, 0);  
    backword();
    delay(100);
    turnRight();
    delay(200);
  } else if (irFrontValue >= 251 && irFrontValue <= 700) {  
    analogWrite(pump, 0);  
    forword();
  } else if (irLeftValue >= 251 && irLeftValue <= 700) {  
    analogWrite(pump, 0);  
    backword();
    delay(100);
    turnLeft();
    delay(200);
  } else {
    analogWrite(pump, 0);  
    Stop();
  }

  delay(10);
}

void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;  
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);  
  digitalWrite(pin, LOW);
  delay(50);  
}

void forword() {
  digitalWrite(IN1, HIGH);  
  digitalWrite(IN2, LOW);   
  digitalWrite(IN3, HIGH);   
  digitalWrite(IN4, LOW);  
}

void backword() {
  digitalWrite(IN1, LOW);   
  digitalWrite(IN2, HIGH);  
  digitalWrite(IN3, LOW);  
  digitalWrite(IN4, HIGH);   
}

void turnRight() {
  digitalWrite(IN1, HIGH);   
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);   
  digitalWrite(IN4, HIGH);  
}

void turnLeft() {
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, HIGH);   
  digitalWrite(IN3, HIGH);  
  digitalWrite(IN4, LOW);   
}

void Stop() {
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, LOW);  
  digitalWrite(IN3, LOW);  
  digitalWrite(IN4, LOW);  
}
