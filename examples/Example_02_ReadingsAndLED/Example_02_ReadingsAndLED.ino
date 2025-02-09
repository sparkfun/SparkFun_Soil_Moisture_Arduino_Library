/*
*---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */

#include "SparkFun_Soil_Moisture_Sensor.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_Soil_Moisture_Sensor
#include <Wire.h>

SparkFunSoilMoistureSensor mySoilSensor; // Create an instance of the sensor class

// Some Dev boards have their QWIIC connector on Wire or Wire1
// This #ifdef will help this sketch work across more products

#ifdef ARDUINO_SPARKFUN_THINGPLUS_RP2040
#define wirePort Wire1
#else
#define wirePort Wire
#endif

//----------------------------------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("SparkFun Soil Sensor Example 2");
    Serial.println("Basic Readings and On-Board LED Control");
    Serial.println("------------------------------------------------------------");
    Serial.println();


    wirePort.begin();

    if (mySoilSensor.begin() == false)
    {
        Serial.println("Soil Moisture Sensor not detected at default I2C address. Verify the sensor is connected. Stopping.");
                       
        while (1)
            ;
    }
    Serial.println("Soil Moisture Sensor found!");

    mySoilSensor.LEDOff();

    Serial.println();
    Serial.println("Reading soil moisture sensor...");
    Serial.println();
}

//----------------------------------------------------------------------------------------

void loop()
{
    // Wink the LED during the reading
    mySoilSensor.LEDOn();
    // Let's get some readings
    uint16_t soilMoisture = mySoilSensor.readMoistureValue();

    // Output the value:
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);

    // small delay for the LED to be seen
    delay(100);
    mySoilSensor.LEDOff();
    
    // delay our reading.
    delay(2000);
}
