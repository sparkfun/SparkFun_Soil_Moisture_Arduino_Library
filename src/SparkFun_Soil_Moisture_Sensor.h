
/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */

 // A simple wrapper around the SparkFun Soil Moisture Sensor - based on the original example driver /example from Zio 
 // from here https://github.com/sparkfun/Zio-Qwiic-Soil-Moisture-Sensor

#pragma once

// To make the Arduino machine happy, include the toolkit header before the core implementation for this device
#include <SparkFun_Toolkit.h>
#include "sfeTk/sfeDevSoilMoisture.h"

// Define our Arduino Object
class SparkFunSoilMoistureSensor : public sfeDevSoilMoisture
{
  public:
    /// @brief Begins the Device
    /// @param address I2C device address to use for the sensor
    /// @param wirePort Wire port to use for I2C communication
    /// @return True if successful, false otherwise
    bool begin(const uint8_t address = SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS, TwoWire &wirePort = Wire)
    {
        // Setup Arduino I2C bus
        _theI2CBus.init(wirePort, address);

        // Begin the sensor and make sure it's connected.
        return sfeDevSoilMoisture::begin(&_theI2CBus) == kSTkErrOk ? isConnected() : false;
    }

    /// @brief Checks if the Device is connected
    /// @return True if the sensor is connected, false otherwise
    bool isConnected()
    {
        return _theI2CBus.ping() == kSTkErrOk;
    }

  private:
    sfeTkArdI2C _theI2CBus;
};
