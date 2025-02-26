/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 * This example demonstrates how to change the I2C address of the Soil Sensor.
 *
 * Supported Sensor:
 *  Qwiic Soil Moisture Sensor https://www.sparkfun.com/sparkfun-qwiic-soil-moisture-sensor.html
 *
 * Hardware Connections:
 * - Connect the Qwiic Soil Moisture Sensor to the Qwiic connector of your development board (SparkFun Thing Plus,
 *ReadBoard ...etc.) If you don't have a development board with a Qwiic connector, you can purchase one at sparkfun.com
 * - Connect the development board to your computer using a USB cable.
 * - Open the Serial Monitor at a baud rate of 115200 to see the sketch output.
 *
 *---------------------------------------------------------------------------------
 */

#include "SparkFun_Soil_Moisture_Sensor.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_Soil_Moisture_Sensor
#include <Wire.h>

SparkFunSoilMoistureSensor mySoilSensor; // Create an instance of the sensor class

// Define our alternate I2C address - the default address + 1
#define ALTERNATE_I2C_ADDRESS SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS + 1

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

    // NOTE:
    // Changing I2C addresses and reconnecting can be a bit fiddily. If the following fails, try resetting the board

    Serial.println();
    Serial.println("------------------------------------------------------------");
    Serial.println("SparkFun Soil Sensor Example 4");
    Serial.println("Change the I2C Address Example");
    Serial.println("------------------------------------------------------------");
    Serial.println();

    wirePort.begin();

    Serial.println("Checking for the sensor at the default I2C address...");

    if (mySoilSensor.begin() == false)
    {
        // Did the sensor get left at the alt address? Try the alt address
        SparkFunSoilMoistureSensorI2C tmpSensor;
        if (tmpSensor.begin(ALTERNATE_I2C_ADDRESS) == true)
        {
            Serial.println("Sensor found at the alternate address. Changing the address back to the default.");
            tmpSensor.setI2CAddress(SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS);
            delay(3000);
            if (mySoilSensor.begin() == false)
            {
                Serial.println(
                    "Error: Soil Moisture Sensor not detected at default I2C address. Verify the sensor is connected. Stopping.");
                while (1)
                    ;
            }
        }
        else
        {
        Serial.println(
            "Soil Moisture Sensor not detected at default or alternative I2C address. Verify the sensor is connected. Stopping.");
        while (1)
            ;
        }
    }
    Serial.println("Soil Moisture Sensor found!");
    Serial.println();

    // write out the current value
    Serial.print("Sensor Address: 0x");
    Serial.print(mySoilSensor.address(), HEX);

    // Output the value:
    Serial.print(", Soil Moisture: ");
    Serial.print(mySoilSensor.readMoistureValue());
    Serial.print(" (sensor value), ");

    // Now the percent moisture
    Serial.print(mySoilSensor.readMoisturePercentage());
    Serial.println("% wet");
    
    Serial.println();

    Serial.println("Changing the I2C address of the sensor to: 0x" + String(ALTERNATE_I2C_ADDRESS, HEX));

    // Change the I2C address
    if (mySoilSensor.setI2CAddress(ALTERNATE_I2C_ADDRESS) != kSTkErrOk)
    {
        Serial.println("ERROR: Unable to change the I2C address. Stopping.");
        while (1)
            ;
    }

    // let the changes take place on the sensor board.
    delay(3000);
    Serial.println();

    // try the sensor at the new address
    SparkFunSoilMoistureSensorI2C mySoilSensorNewAddress; // Create an instance of the sensor class at new address

    if (mySoilSensorNewAddress.begin(ALTERNATE_I2C_ADDRESS) == false)
    {
        Serial.println(
            "Error: Soil Moisture Sensor not detected at new I2C address. Stopping.");
        while (1)
            ;
    }
    Serial.println("Soil Moisture Sensor found at the new address!");
    Serial.println();;

    // write out the current value
    Serial.print("Sensor Address: 0x");
    Serial.print(mySoilSensorNewAddress.address(), HEX);

    // Output the value:
    Serial.print(", Soil Moisture: ");
    Serial.print(mySoilSensorNewAddress.readMoistureValue());
    Serial.print(" (sensor value), ");

    // Now the percent moisture
    Serial.print(mySoilSensorNewAddress.readMoisturePercentage());
    Serial.println("% wet");
    

    Serial.println("Changing the I2C address of the sensor to: 0x" + String(SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS, HEX));

    // Change the I2C address
    if (mySoilSensorNewAddress.setI2CAddress(SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS) != kSTkErrOk)
    {
        Serial.println("ERROR: Unable to change the I2C address. Stopping.");
        while (1)
            ;
    }

    // let the changes take place on the sensor board.
    delay(3000);

    Serial.println();
    Serial.println("Address changing complete. Entering loop...");
    Serial.println();    

}

//----------------------------------------------------------------------------------------
void loop()
{
    // write out the current value
    Serial.print("Sensor Address: 0x");
    Serial.print(mySoilSensor.address(), HEX);
    Serial.print(", Soil Moisture: ");
    Serial.print(mySoilSensor.readMoistureValue());
    Serial.print(" (sensor value), ");

    // Now the percent moisture
    Serial.print(mySoilSensor.readMoisturePercentage());
    Serial.println("% wet");

    // loop delay 
    delay(5000);
}
