#include <SoftwareSerial.h>

SoftwareSerial SIM800L(7, 8); // RX, TX

void setup()
{
  SIM800L.begin(9600); // Setting the baud rate of SIM800L
  delay(1000);
  SIM800L.println("AT+CMGF=1"); // Configuring TEXT mode
  delay(1000);
  SIM800L.println("AT+CNMI=2,2,0,0,0"); // Configuring SMS notifications
  delay(1000);
}

void loop()
{
  if (SIM800L.available())
  {
    String message = SIM800L.readString();
    if (message.indexOf("Hi") >= 0)
    {
      sendSMS();
    }
  }
}

void sendSMS()
{
  SIM800L.println("AT+CMGS=\"+919700104930\""); // Change to the phone number you want to send to
  delay(1000);
  SIM800L.println("Hello"); // The SMS text you want to send
  delay(1000);
  SIM800L.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
}