/**
 * Async example for MPL3115A2
 */

#include <Adafruit_MPL3115A2.h>

Adafruit_MPL3115A2 mpl;

float tempC=0.0;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Adafruit_MPL3115A2 test!");

  if (!mpl.begin()) {
    Serial.println("Could not find sensor. Check wiring.");
    while(1);
  }

  // set mode before starting a conversion
  Serial.println("Setting mode to barometer (pressure).");
  mpl.setMode(MPL3115A2_BAROMETER);
}

void loop() {
  // start a conversion
  mpl.startOneShot();

  Serial.print("Temperature = ");
  //Serial.println(mpl.getLastConversionResults(MPL3115A2_TEMPERATURE));
  tempC=mpl.getLastConversionResults(MPL3115A2_TEMPERATURE);
  Serial.println(tempC);

  delay(1000);
}