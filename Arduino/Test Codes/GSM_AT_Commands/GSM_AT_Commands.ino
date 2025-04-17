#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial sim900a(7, 8); //SIM800L Tx & Rx is connected to Arduino #3 & #2

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  sim900a.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  sim900a.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  sim900a.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  sim900a.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  sim900a.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  
}

void loop()
{//sim900a.println("AT+CSQ");
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    sim900a.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim900a.available()) 
  {
    Serial.write(sim900a.read());//Forward what Software Serial received to Serial Port
  }
}