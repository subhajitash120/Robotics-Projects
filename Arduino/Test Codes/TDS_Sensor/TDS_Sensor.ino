#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define TDS_SENSOR_PIN A0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Dallas Temperature and TDS Sensor Demo");
  sensors.begin();
}

float getTDSCorrection(float temperatureC)
{
  // Add your temperature compensation logic here
  // You can replace this with your specific TDS compensation algorithm
  // For example, if TDS increases with temperature, you can add a positive correction factor.
  float temperatureCorrection = 0.1 * temperatureC; // Adjust this factor as needed
  return temperatureCorrection;
}

void loop(void)
{
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");

  float tempC = sensors.getTempCByIndex(0);

  if (tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.print(tempC);
    
    // Read TDS sensor value
    int tdsSensorValue = analogRead(TDS_SENSOR_PIN);
    
    // Apply temperature compensation
    float temperatureCompensatedTDS = tdsSensorValue + getTDSCorrection(tempC);

    Serial.print(", TDS Sensor Value: ");
    Serial.println(temperatureCompensatedTDS);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }

  // Add a suitable delay between readings
  delay(5000); // 5 seconds delay (adjust as needed)
}
