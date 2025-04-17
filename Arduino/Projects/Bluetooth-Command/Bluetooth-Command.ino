#include <SoftwareSerial.h> // Include the SoftwareSerial library for using software serial communication

#define RELAY_1 11 // Define the pin for relay 1
#define RELAY_2 10 // Define the pin for relay 2
#define RELAY_3 9  // Define the pin for relay 3
#define RELAY_4 8  // Define the pin for relay 4

SoftwareSerial mySerial(3, 2); // Initialize a SoftwareSerial object with RX pin 3 and TX pin 2

void setup()
{
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate for debugging
  mySerial.begin(9600); // Initialize the software serial communication with the same baud rate
  Serial.println("Initializing..."); // Print initialization message to serial monitor
  Serial.println("The device started, now you can pair it with Bluetooth!"); // Print pairing message to serial monitor

  // Set relay pins as output pins
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);

  // Turn off all relays initially
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
}

void loop()
{
  if (mySerial.available()) // Check if there is data available to read from Bluetooth
  {
    // Read the incoming command from Bluetooth and trim whitespace characters
    String command = mySerial.readStringUntil('\n');
    command.trim();
    command.toLowerCase(); // Convert the command to lowercase for case-insensitive comparison

    // Check the received command and control relays accordingly
    if (command.equals("turn on tubelight"))
    {
      digitalWrite(RELAY_1, LOW);
    }
    else if (command.equals("turn off tubelight"))
    {
      digitalWrite(RELAY_1, HIGH);
    }

    if (command.equals("turn on bed lamp"))
    {
      digitalWrite(RELAY_2, LOW);
    }
    else if (command.equals("turn off bed lamp"))
    {
      digitalWrite(RELAY_2, HIGH);
    }

    if (command.equals("turn on fan"))
    {
      digitalWrite(RELAY_3, LOW);
    }
    else if (command.equals("turn off fan"))
    {
      digitalWrite(RELAY_3, HIGH);
    }

    if (command.equals("turn on bell"))
    {
      digitalWrite(RELAY_4, LOW);
    }
    else if (command.equals("turn off bell"))
    {
      digitalWrite(RELAY_4, HIGH);
    }

    // Turn on/off all relays based on the received command
    if (command.equals("turn on all appliances"))
    {
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      digitalWrite(RELAY_3, LOW);
      digitalWrite(RELAY_4, LOW);
    }
    else if (command.equals("turn off all appliances"))
    {
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);
      digitalWrite(RELAY_3, HIGH);
      digitalWrite(RELAY_4, HIGH);
    }
    else
    {
      Serial.println("Invalid command."); // Print error message for invalid commands
    }
  }
}
