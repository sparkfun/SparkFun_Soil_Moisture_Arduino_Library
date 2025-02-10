/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */

// This example demonstrates how to read the soil moisture sensor and control the on-board LED.
// The LED flash rate is based on how "wet" the soil is. The drier the soil, the faster the LED flashes.

#include "SparkFun_Soil_Moisture_Sensor.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_Soil_Moisture_Sensor
#include <Wire.h>

SparkFunSoilMoistureSensorI2C mySoilSensor; // Create an instance of the sensor class

// The plan:
//    The value of the sensor has the following range:
//       0    = 100% wet soil (no resistance)
//       1024 = 0% wet soil (infinite resistance) note: the sensors ADC is 10-bit 2^10 = 1024
//
//    The LED will flash faster the drier the soil is. The LED will be effectively off when the soil is 100% wet.
//
// The flash rate is:
//  0% Wet = 1 flash every <fast_value> seconds
/// 10%-90% Wet = rate proportional to the percentage of wetness
//  100% Wet = off
//
// To do this, the following is done:
//  1. Get the sensor value as a ration of moisture (0 - 1.0)
//  2. Calculate the percentage of wetness and multiply by 10 using integer math (floor)- this gives a value of 0 to 10
//  3. The LED flash_rate = value * increment_value, if value is 0 (0% moisture), rate = <fast_value> seconds. If the
//     value >= 90% (9 or 10), rate = <long_value> seconds. Otherwise, rate = value * increment_value

// Min flash rate in ms (2 seconds)
#define FAST_FLASH_RATE 2000

// Max flash rate in ms (1 day) - basically off
#define LONG_FLASH_RATE 86400000

// Define our increment value = 10000ms (10 seconds)
#define FLASH_INCREMENT 10000

// Time between loop iterations - 5 second (should not be greater that FAST_FLASH_RATE)
#define LOOP_DELAY FAST_FLASH_RATE

// Define a variable for the last blink time - in ms since boot
uint32_t lastBlinkTime = 0;

// Define a variable for the blink rate - delay between blinks in ms
uint32_t blinkRate = 0;

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

    // Set up the serial port for output
    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    // print out the startup banner
    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("SparkFun Soil Sensor Example 3");
    Serial.println("Blink sensor LED based on soil moisture percentage");
    Serial.println("------------------------------------------------------------");
    Serial.println();

    // start the I2C bus
    wirePort.begin();

    // Check if the sensor is connected and initialize it
    if (mySoilSensor.begin() == false)
    {
        Serial.println(
            "Soil Moisture Sensor not detected at default I2C address. Verify the sensor is connected. Stopping.");

        while (1)
            ;
    }

    Serial.println("Soil Moisture Sensor found!");

    // print out the LED flash rate information
    Serial.println();
    Serial.println("LED will flash based on the soil moisture reading:");
    Serial.print("  < 10% Wet = 1 flash every ");
    Serial.print(FAST_FLASH_RATE / 1000);
    Serial.println(" seconds");
    Serial.print("  10%-90% Wet = flash rate proportional to the percentage of wetness: ");
    Serial.print(FLASH_INCREMENT / 1000);
    Serial.println(" seconds per 10% range");
    Serial.println("  > 90% Wet = LED off");
    Serial.println();

    mySoilSensor.LEDOff();

    // setup the initial blink rate bin - off
    blinkRate = LONG_FLASH_RATE;

    // initial last blink time - now!
    lastBlinkTime = millis();

    Serial.println("Reading soil moisture sensor...");
    Serial.println();
}

//----------------------------------------------------------------------------------------
// A function to get the blink rate based on the soil moisture ration (0 - 1.0)
uint32_t getBlinkRate(float wetRatio)
{
    // our return value
    uint32_t newBlinkRate = 0;

    // Calculate the delay rate "bin" (wet ratio * 10) for the LED based on this current reading
    // The rate bin is a value from 0 to 10
    uint32_t blinkRateBin = wetRatio * 10;

    // Determine the blink rate based on the bin
    switch (blinkRateBin)
    {
    case 0:
        // 0% wet - Flash at the fast rate
        newBlinkRate = FAST_FLASH_RATE;
        break;
    case 9:
    case 10:
        // rate > 80% (90% - 100%)
        newBlinkRate = LONG_FLASH_RATE; // 1 day - basically off
        break;
    default:
        // 10%-90% wet - rate proportional to the percentage of wetness
        newBlinkRate = blinkRateBin * FLASH_INCREMENT;
    }

    return newBlinkRate;
}

//----------------------------------------------------------------------------------------
// A function to check if we need to flash the LED based on elapsed time from last blink
void checkForLEDBlink(void)
{
    // do we need to flash the LED?
    if (millis() - lastBlinkTime > blinkRate)
    {
        // Wink the LED during the reading
        mySoilSensor.LEDOn();
        delay(100);
        mySoilSensor.LEDOff();

        lastBlinkTime = millis();
    }
}
//----------------------------------------------------------------------------------------
void loop()
{
    // Output the value:
    Serial.print("Soil Moisture: ");
    Serial.print(mySoilSensor.readMoistureValue());
    Serial.print(" (sensor value), ");

    // Now the percent moisture
    Serial.print(mySoilSensor.readMoisturePercentage());
    Serial.println("% wet");

    // The current blink rate based on the soil moisture ratio reading
    uint32_t newBlinkRate = getBlinkRate(mySoilSensor.readMoistureRatio());

    // update the blink rate if it has changed
    if (newBlinkRate != blinkRate)
    {
        blinkRate = newBlinkRate;
        Serial.print("New blink delay: ");
        Serial.print(blinkRate);
        Serial.println(" ms");
    }
    // check if we need to flash the LED
    checkForLEDBlink();

    // delay our loop.
    delay(LOOP_DELAY);
}
