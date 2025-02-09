
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

// Note:
// The core of the implementation for this device library is in the SparkFun Toolkit object sfeDevSoilMoisture,
// contained in the directory sfeTk. This object implements all functionality independent of the bus type, I2C or SPI.
// The SparkFunSoilMoistureSensorI2C and SparkFunSoilMoistureSensorSPI classes below are the Arduino-specific, bus-specific
// implementations that inherit from the core toolkit class. 
//
//-----------------------------------------------------------------------------------------------
// Define our Arduino Object - I2C version
class SparkFunSoilMoistureSensorI2C : public sfeDevSoilMoisture
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

//-----------------------------------------------------------------------------------------------
// Define our Arduino Object - SPI version
class SparkFunSoilMoistureSensorSPI : public sfeDevSoilMoisture
{
  public:
    /// @brief Begins the Device with SPI as the communication bus
    /// @param csPin The chip select pin for the sensor
    /// @param spiPort The SPI port to use for communication
    /// @param spiSettings The SPI settings to use for communication
    /// @return True if successful, false otherwise
    bool begin(const uint8_t csPin, SPIClass &spiPort = SPI, SPISettings spiSettings = SPISettings(100000, MSBFIRST, SPI_MODE0))
    {

        // Setup Arduino SPI bus
        _theSPIBus.init(spiPort, spiSettings, csPin, true);

        // Begin the sensor and make sure it's connected.
        return (sfeDevSoilMoisture::begin(&_theSPIBus) == kSTkErrOk );
    }

  private:
    sfeTkArdSPI _theSPIBus;
};