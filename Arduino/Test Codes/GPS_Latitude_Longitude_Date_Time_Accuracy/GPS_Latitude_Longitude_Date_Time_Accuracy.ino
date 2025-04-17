#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Define SoftwareSerial for NEO-6M GPS module
SoftwareSerial gpsSerial(10, 11); // RX, TX
TinyGPSPlus gps;

// Custom objects to get HDOP and VDOP from GPGSA sentence
TinyGPSCustom hdop(gps, "GPGSA", 16);
TinyGPSCustom vdop(gps, "GPGSA", 17);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  // Read GPS data from the NEO-6M module
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // Display Indian time after successful GPS data decoding
      displayIndianTime();
    }
  }
}

void displayIndianTime() {
  // Check if there is enough satellite data for an accurate fix
  if (gps.satellites.value() > 3) {
  
    // Convert hdop and vdop values to float
    float hdopValue = atof(hdop.value());
    float vdopValue = atof(vdop.value());

    // Calculate accuracy using the provided formula
    float accuracy = (hdopValue * vdopValue) * (22.0 / gps.satellites.value());

    // Adjust time to Indian Standard Time (IST) - UTC+5:30
    TinyGPSTime gpsTime = gps.time;
    int hourIST = gpsTime.hour() + 5;
    int minuteIST = gpsTime.minute() + 30;

    // Handle overflow
    if (minuteIST >= 60) {
      hourIST += 1;
      minuteIST -= 60;
    }

    // Convert to 12-hour format
    int hour12 = hourIST % 12;
    if (hour12 == 0) {
      hour12 = 12; // Set 12 for midnight
    }

    // Determine AM or PM
    String ampm = (hourIST < 12) ? "AM" : "PM";

    // Print latitude, longitude, date, time, and accuracy
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Date: ");
    printDigits(gps.date.day());
    Serial.print("-");
    printDigits(gps.date.month());
    Serial.print("-");
    Serial.println(gps.date.year());
    Serial.print("Time: ");
    Serial.print(hour12);
    Serial.print(":");
    printDigits(minuteIST);
    Serial.print(" ");
    Serial.print(ampm);
    Serial.println();
   Serial.print("Accuracy: ");
    Serial.print(accuracy, 2);
    Serial.println(" m^2");
    Serial.println();
    delay(1000);
  } else {
    Serial.println("Not enough satellites for an accurate fix");
  }
}

void printDigits(int digits) {
  // Add leading zero if needed
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
