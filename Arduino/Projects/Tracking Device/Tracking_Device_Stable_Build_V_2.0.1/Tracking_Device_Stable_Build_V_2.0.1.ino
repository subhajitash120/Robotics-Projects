#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Define pins for GSM and GPS modules
#define GSM_RX 7
#define GSM_TX 8
#define GPS_RX 0
#define GPS_TX 1

// Create a SoftwareSerial object for GSM communication
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

// Create TinyGPS++ objects for GPS data parsing
TinyGPSPlus gps;
TinyGPSCustom hdop(gps, "GPGSA", 16);
TinyGPSCustom vdop(gps, "GPGSA", 17);

// Flag to track whether an SMS has been sent
bool smsSent = false;

// Setup function, runs once at the beginning
void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize GSM communication
  gsmSerial.begin(9600);

  // Configure GSM module
  gsmConfig();

  // Debug message indicating system readiness
  Serial.println("System ready!");
}

// Main loop function, runs repeatedly
void loop() {
  // Check for incoming SMS and update the 'smsSent' flag
  checkSMS();

  // Process GPS data received from the serial port
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      // Check if GPS location is valid
      if (gps.location.isValid()) {
        // Check if accuracy is available and an SMS hasn't been sent
        if (gps.satellites.value() > 3 && !smsSent) {
          // Wait for accuracy to be acquired
          float accuracy = getAccuracy();
          if (accuracy > 0.0) {
            // Send the GPS information via SMS and print to serial
            sendResponse(gps.location.lat(), gps.location.lng(), accuracy);
            smsSent = true;
          }
        }
      }
    }
  }

  // Print information to serial monitor every 3 seconds
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime >= 3000) {
    lastPrintTime = millis();

    // Query GSM signal strength
    gsmSerial.println("AT+CSQ");
    delay(1000);

    // Parse and retrieve GSM signal strength
    int signalStrength = -1; // Default value if not found
    if (gsmSerial.find("+CSQ: ")) {
      signalStrength = gsmSerial.parseInt();
    }

    // Create a formatted message with GPS and GSM information
    String date = getFormattedDate(gps.date.day(), gps.date.month(), gps.date.year());
    String time = getFormattedTimeIST(gps.time.hour(), gps.time.minute());
    
    Serial.print("Accuracy: ");
    Serial.print(getAccuracy(), 2);
    Serial.print(" meters, GSM Signal Strength: ");
    Serial.print(signalStrength);
    Serial.print(", Time: ");
    Serial.print(time);
    Serial.print(", Date: ");
    Serial.print(date);
    Serial.print(", Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Longitude: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print(", URL: ");
    Serial.println("http://maps.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6));
  }
}


// Function to check for incoming SMS containing "Track"
void checkSMS() {
  if (gsmSerial.available()) {
    String sms = gsmSerial.readString();
    if (sms.indexOf("Track") >= 0) {
      smsSent = false;
    }
  }
}

// Function to send GPS information via SMS and print to serial
void sendResponse(float latitude, float longitude, float accuracy) {
  String message = createMessage(latitude, longitude, accuracy);

  // Send SMS to a predefined phone number
  gsmSerial.println("AT+CMGS=\"+919700104930\"");
  delay(1000);
  gsmSerial.print(message);
  gsmSerial.write(26);
  delay(1000);

  // Debug message indicating that an SMS has been sent
  Serial.println("Sent SMS: " + message);
  Serial.println();
}

// Function to print GPS information to serial
void printGPSInfo() {
  String message = createMessage(gps.location.lat(), gps.location.lng(), getAccuracy());

  Serial.println(message);
}

// Function to create a formatted message with GPS and GSM information
String createMessage(float latitude, float longitude, float accuracy) {
  // Query GSM signal strength
  gsmSerial.println("AT+CSQ");
  delay(1000);

  // Parse and retrieve GSM signal strength
  if (gsmSerial.find("+CSQ: ")) {
    int signalStrength = gsmSerial.parseInt();

    // Create a formatted message with GPS and GSM information
    String date = getFormattedDate(gps.date.day(), gps.date.month(), gps.date.year());
    String time = getFormattedTimeIST(gps.time.hour(), gps.time.minute());

    String message = "Accuracy: " + String(accuracy, 2) + " meters\n";
    message += "GSM Signal Strength: " + String(signalStrength) + "\n";
    message += "Time: " + time + "\n";
    message += "Date: " + date + "\n";
    message += "Latitude: " + String(latitude, 6) + ", Longitude: " + String(longitude, 6) + "\n";
    String gmapsURL = "http://maps.google.com/maps?q=" + String(latitude, 6) + "," + String(longitude, 6);
    message += "Google Maps URL: " + gmapsURL;

    return message;
  }

  return "";
}

// Function to format the date
String getFormattedDate(int day, int month, int year) {
  String monthNames[] = {"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  String formattedDate = String(day) + "-" + monthNames[month] + "-" + String(year);
  return formattedDate;
}

// Function to format the time in IST (Indian Standard Time)
String getFormattedTimeIST(int hour, int minute) {
  hour = hour + 5;
  minute = minute + 30;

  if (minute >= 60) {
    minute -= 60;
    hour += 1;
  }

  hour = hour % 24;

  String period = (hour < 12) ? "AM" : "PM";
  hour = (hour > 12) ? (hour - 12) : hour;
  hour = (hour == 0) ? 12 : hour;
  String formattedTime = String(hour) + ":" + (minute < 10 ? "0" : "") + String(minute) + " " + period;
  return formattedTime;
}

// Function to calculate GPS accuracy
float getAccuracy() {
  if (gps.satellites.value() > 3) {
    float hdopValue = atof(hdop.value());
    float vdopValue = atof(vdop.value());
    float accuracy = (hdopValue * vdopValue) * (22.0 / gps.satellites.value());
    delay(1000);
    return accuracy;
  } else {
    return -1.0;
  }
}
