#include "EmonLib.h"

const float vCalibration = 41.5;
const float currCalibration = 0.15;

EnergyMonitor emon;

void setup()
{
  Serial.begin(115200);
  emon.voltage(35, vCalibration, 1.7);
  emon.current(34, currCalibration);
  delay(1000);
}

void loop()
{
  emon.calcVI(20, 2000);
  Serial.printf("Vrms: %.2fV\tIrms: %.4fA\tPower: %.4fW\n",
                emon.Vrms, emon.Irms, emon.apparentPower);
  delay(5000);
}
