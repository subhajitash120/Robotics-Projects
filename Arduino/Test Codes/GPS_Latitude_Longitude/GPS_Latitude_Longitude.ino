#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Define SoftwareSerial for NEO-6M GPS module
SoftwareSerial gpsSerial(13, 15); // RX, TX
TinyGPSPlus gps;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  // Read GPS data from the NEO-6M module
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      displayLatLong();
    }
  }
}

void displayLatLong() {
  // Check if there is enough satellite data for an accurate fix
  if (gps.satellites.value() > 3) {
    // Print latitude and longitude
    Serial.print("Latitude: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.println();
    delay(1000);
  } else {
    Serial.println("Not enough satellites for an accurate fix");
  }
}
