#include <SoftwareSerial.h>

#define LIGHT 11
#define WATER_PUMP 10
#define DOOR_LOCK 9
#define BUZZER 8  // Define buzzer pin

SoftwareSerial mySerial(3, 2);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  Serial.println("The device started, now you can pair it with Bluetooth!");

  pinMode(LIGHT, OUTPUT);
  pinMode(WATER_PUMP, OUTPUT);
  pinMode(DOOR_LOCK, OUTPUT);
  pinMode(BUZZER, OUTPUT);  // Set buzzer pin as output

  digitalWrite(LIGHT, HIGH);
  digitalWrite(WATER_PUMP, HIGH);
  digitalWrite(DOOR_LOCK, HIGH);
  digitalWrite(BUZZER, HIGH);  // Ensure the buzzer is off initially
}

void loop() {
  if (mySerial.available()) {
    String command = mySerial.readStringUntil('\n');
    command.trim();
    command.toLowerCase();

    if (command.equals("turn on light")) {
      digitalWrite(LIGHT, LOW);
    } else if (command.equals("turn off light")) {
      digitalWrite(LIGHT, HIGH);
    }

    if (command.equals("turn on water pump")) {
      digitalWrite(WATER_PUMP, LOW);
    } else if (command.equals("turn off water pump")) {
      digitalWrite(WATER_PUMP, HIGH);
    }

    if (command.equals("open the door")) {
      digitalWrite(DOOR_LOCK, LOW);
    } else if (command.equals("close the door")) {
      digitalWrite(DOOR_LOCK, HIGH);
    }

    if (command.equals("emergency")) {
      digitalWrite(BUZZER, LOW);  // Activate buzzer
    } else if (command.equals("all safe")) {
      digitalWrite(BUZZER, HIGH);  // Deactivate buzzer
    }

    if (command.equals("turn on all appliances")) {
      digitalWrite(LIGHT, LOW);
      digitalWrite(WATER_PUMP, LOW);
      digitalWrite(DOOR_LOCK, LOW);
      digitalWrite(BUZZER, LOW);  // Activate all appliances including the buzzer
    } else if (command.equals("turn off all appliances")) {
      digitalWrite(LIGHT, HIGH);
      digitalWrite(WATER_PUMP, HIGH);
      digitalWrite(DOOR_LOCK, HIGH);
      digitalWrite(BUZZER, HIGH);  // Deactivate all appliances including the buzzer
    } else {
      Serial.println("Invalid command.");
    }
  }
}
