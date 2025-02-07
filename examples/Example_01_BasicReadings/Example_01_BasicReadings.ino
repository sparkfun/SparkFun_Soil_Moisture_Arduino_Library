/*

*/

#include "SparkFun_Soil_Moisture_Sensor.h" // CTRL+Click here to get the library: http://librarymanager/All#SparkFun_Soil_Moisture
#include <Wire.h>

SparkFunSoilMoistureSensor mySoilSensor; // Create an instance of the sensor class

// Some Dev boards have their QWIIC connector on Wire or Wire1
// This #ifdef will help this sketch work across more products

#ifdef ARDUINO_SPARKFUN_THINGPLUS_RP2040
#define wirePort Wire1
#else
#define wirePort Wire
#endif

void setup()
{
    Serial.begin(115200);

    while (!Serial)
        delay(10); // Wait for Serial to become available.
    // Necessary for boards with native USB (like the SAMD51 Thing+).
    // For a final version of a project that does not need serial debug (or a USB cable plugged in),
    // Comment out this while loop, or it will prevent the remaining code from running.

    Serial.println();
    Serial.println("Soil Sensor Example 1 - Basic Readings");

    wirePort.begin();

    if (mySoilSensor.begin() == false)
    {
        Serial.println("Soil Moisture Sensor not detected at default I2C address. Verify the sensor is connected "
                       "correctly. Freezing...");
        while (1)
            ;
    }
    Serial.println("Soil Moisture Sensor found!");
}

void loop()
{
    // Let's get some readings
    uint16_t soilMoisture = mySoilSensor.readMoistureValue();
    Serial.print("Soil Moisture: ");
    Serial.println(soilMoisture);

    delay(2000);
}
