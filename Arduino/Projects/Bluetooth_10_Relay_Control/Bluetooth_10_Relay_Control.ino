#include <SoftwareSerial.h>

#define RELAY_1 11
#define RELAY_2 10
#define RELAY_3 9
#define RELAY_4 8
#define RELAY_5 7
#define RELAY_6 6
#define RELAY_7 5
#define RELAY_8 4
#define RELAY_9 3
#define RELAY_10 2

SoftwareSerial mySerial(12, 13);

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Initializing...");
  Serial.println("The device started, now you can pair it with Bluetooth!");

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RELAY_5, OUTPUT);
  pinMode(RELAY_6, OUTPUT);
  pinMode(RELAY_7, OUTPUT);
  pinMode(RELAY_8, OUTPUT);
  pinMode(RELAY_9, OUTPUT);
  pinMode(RELAY_10, OUTPUT);

  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
  digitalWrite(RELAY_5, HIGH);
  digitalWrite(RELAY_6, HIGH);
  digitalWrite(RELAY_7, HIGH);
  digitalWrite(RELAY_8, HIGH);
  digitalWrite(RELAY_9, HIGH);
  digitalWrite(RELAY_10, HIGH);
}

void loop() {
  if (mySerial.available()) {
    String command = mySerial.readStringUntil('\n');
    command.trim();
  

    if (command.equals("A")) {
      digitalWrite(RELAY_1, LOW);
    } else if (command.equals("a")) {
      digitalWrite(RELAY_1, HIGH);
    }

    if (command.equals("B")) {
      digitalWrite(RELAY_2, LOW);
    } else if (command.equals("b")) {
      digitalWrite(RELAY_2, HIGH);
    }

    if (command.equals("C")) {
      digitalWrite(RELAY_3, LOW);
    } else if (command.equals("c")) {
      digitalWrite(RELAY_3, HIGH);
    }

    if (command.equals("D")) {
      digitalWrite(RELAY_4, LOW);
    } else if (command.equals("d")) {
      digitalWrite(RELAY_4, HIGH);
    }

    if (command.equals("E")) {
      digitalWrite(RELAY_5, LOW);
    } else if (command.equals("e")) {
      digitalWrite(RELAY_5, HIGH);
    }

    if (command.equals("F")) {
      digitalWrite(RELAY_6, LOW);
    } else if (command.equals("f")) {
      digitalWrite(RELAY_6, HIGH);
    }

    if (command.equals("G")) {
      digitalWrite(RELAY_7, LOW);
    } else if (command.equals("g")) {
      digitalWrite(RELAY_7, HIGH);
    }

    if (command.equals("H")) {
      digitalWrite(RELAY_8, LOW);
    } else if (command.equals("h")) {
      digitalWrite(RELAY_8, HIGH);
    }

    if (command.equals("I")) {
      digitalWrite(RELAY_9, LOW);
    } else if (command.equals("i")) {
      digitalWrite(RELAY_9, HIGH);
    }

    if (command.equals("J")) {
      digitalWrite(RELAY_10, LOW);
    } else if (command.equals("j")) {
      digitalWrite(RELAY_10, HIGH);
    }

    if (command.equals("K")) {
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);
      digitalWrite(RELAY_5, LOW);
    } else if (command.equals("k")) {
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);
      digitalWrite(RELAY_5, HIGH);
    }

    if (command.equals("L")) {
      digitalWrite(RELAY_6, LOW);
      digitalWrite(RELAY_7, LOW);
      digitalWrite(RELAY_8, LOW);
      digitalWrite(RELAY_9, LOW);
      digitalWrite(RELAY_10, LOW);
    } else if (command.equals("l")) {
      digitalWrite(RELAY_6, HIGH);
      digitalWrite(RELAY_7, HIGH);
      digitalWrite(RELAY_8, HIGH);
      digitalWrite(RELAY_9, HIGH);
      digitalWrite(RELAY_10, HIGH);
    } else {
      Serial.println("Invalid command.");
    }
  }
}
