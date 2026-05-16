/*
  Library for the Sensirion SDP3x Differential Pressure Sensor
  By: Paul Clark
  SparkFun Electronics
  Date: January 18th, 2021
  License: please see LICENSE.md for details

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/17874
*/

#include <SparkFun_SDP3x_Arduino_Library.h> // Click here to get the library: http://librarymanager/All#SparkFun_SDP3x
#include <Wire.h>

SDP3X mySensor; //create an object of the SDP3X class

void setup()
{
  Serial.begin(115200);
  Serial.println(F("SDP3X Example"));

  Wire.begin();

  //mySensor.enableDebugging(); // Uncomment this line to print useful debug messages to Serial

  //Initialize sensor using an alternate I2C address
  if (mySensor.begin(0x22) == false) // Use 0x22 instead of the default (0x21)
  {
    Serial.println(F("SDP3X not detected. Check connections. Freezing..."));
    while (1)
      ; // Do nothing more
  }
}

void loop()
{
  float diffPressure; // Storage for the differential pressure
  float temperature; // Storage for the temperature

  // The datasheet says:
  // "The SDP3x is temperature compensated both for differential pressure and for mass flow compensated differential pressure.
  // In use cases where the SDP3x is used to measure mass flow it is advised to use mass flow temperature compensation.
  // In this case no absolute pressure compensation is required."

  // In this example, we are going to request a one-off (triggered) differential pressure measurement
  // with mass flow temperature compensation and without using clock-stretching (these are the defaults for triggeredMeasurement)
  mySensor.triggeredMeasurement(); //Request a one-off (triggered) measurement using the defaults for massFlow and clockStretching

  delay(45); // From the SDP3x datasheet: the measurement can take 45ms

  // Now we read the differential pressure and temperature from the sensor
  mySensor.readMeasurement(&diffPressure, &temperature); // Read the measurement

  Serial.print(F("Differential pressure is: "));
  Serial.print(diffPressure, 2);
  Serial.print(F(" (Pa)  Temperature is: "));
  Serial.print(temperature, 2);
  Serial.println(F(" (C)"));

  delay(250); //Wait 0.25 second
}
