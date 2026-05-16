/*
  Library for the Sensirion SDP3x Differential Pressure Sensor
  By: Paul Clark
  SparkFun Electronics
  Date: January 18th, 2021
  License: please see LICENSE.md for details

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17874

  Hardware Connections:
  Use a Qwiic cable to connect the Sound Trigger to your board.

  LED_BUILTIN will light up if the SDP31 is detected correctly
  and the temperature and pressure are within bounds.

*/

#include <SparkFun_SDP3x_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_SDP3x
#include <Wire.h>

SDP3X mySensor; //create an object of the SDP3X class

void setup()
{
  Serial.begin(115200);
  Serial.println(F("SDP3X Production Test"));

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  delay(250); // Try every 0.25 seconds

  Wire.begin();

  Wire.beginTransmission(0x21); // Detect SDP31 on address 0x21
  if (Wire.endTransmission() != 0)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: nothing detected on address 0x21"));
    return;
  }
  
  //Initialize sensor
  if (mySensor.begin() == false)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: .begin failed"));
    return;
  }

  float diffPressure; // Storage for the differential pressure
  float temperature; // Storage for the temperature

  delay(25); // From the SDP3x datasheet: start-up time is 25ms

  mySensor.triggeredMeasurement(); //Request a one-off (triggered) measurement using the defaults for massFlow and clockStretching

  delay(45); // From the SDP3x datasheet: the measurement can take 45ms

  // Now we read the differential pressure and temperature from the sensor
  mySensor.readMeasurement(&diffPressure, &temperature); // Read the measurement

  // Check the temperature is within bounds
  if ((temperature < 10.0) || (temperature > 30.0))
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: invalid temperature"));
    return;
  }

  // Check the pressure is within bounds
  if ((diffPressure < -10.0) || (diffPressure > 10.0))
  {
    digitalWrite(LED_BUILTIN, LOW);
    Wire.end();
    Serial.println(F("Error: invalid differential pressure"));
    return;
  }

  // All good!
  Serial.println(F("Test passed!"));
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.end();
}
