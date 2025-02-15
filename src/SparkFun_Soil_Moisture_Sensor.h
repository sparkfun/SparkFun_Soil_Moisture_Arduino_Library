
/*!
 * @file SparkFun_Soil_Moisture_Sensor.h
 * @brief Arduino header file for the SparkFun Soil Moisture Sensor Arduino library
 *
 * This file contains the class definitions for the SparkFun Soil Moisture Sensor Arduino library,
 * including methods for initialization, reading moisture values, and controlling the on-board LED.
 * It provides both I2C and SPI implementations.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

// A simple wrapper around the SparkFun Soil Moisture Sensor - based on the original example driver /example from Zio
// from here https://github.com/sparkfun/Zio-Qwiic-Soil-Moisture-Sensor

#pragma once

// To make the Arduino machine happy, include the toolkit header before the core implementation for this device
// clang-format off
#include <SparkFun_Toolkit.h>
#include "sfTk/sfDevSoilMoisture.h"
// clang-format on

// Note:
// The core of the implementation for this device library is in the SparkFun Toolkit object sfDevSoilMoisture,
// contained in the directory sfTk. This object implements all functionality independent of the bus type, I2C or SPI.
// The SparkFunSoilMoistureSensorI2C and SparkFunSoilMoistureSensorSPI classes below are the Arduino-specific,
// bus-specific implementations that inherit from the core toolkit class.
//
//-----------------------------------------------------------------------------------------------
// Define our Arduino Object - I2C version
class SparkFunSoilMoistureSensor : public sfDevSoilMoisture
{
  public:
    /**
     * @brief Begins the Device
     *
     * Initializes the soil moisture sensor for I2C communication using the specified
     * I2C address and wire port.
     *
     * @param address I2C device address to use for the sensor (default is SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS)
     * @param wirePort Wire port to use for I2C communication (default is Wire)
     * @return true if successful, false otherwise
     */
    bool begin(const uint8_t address = SF_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS, TwoWire &wirePort = Wire)
    {
        // Setup Arduino I2C bus
        _theI2CBus.init(wirePort, address);

        // Begin the sensor and make sure it's connected.
        return sfDevSoilMoisture::begin(&_theI2CBus) == ksfTkErrOk ? isConnected() : false;
    }

    /**
     * @brief Checks if the device is connected and responding
     *
     * Tests communication with the sensor by attempting to ping it over I2C.
     * A successful ping indicates the sensor is properly connected and responding.
     *
     * @return true if the sensor responds to communication attempts, false otherwise
     */
    bool isConnected()
    {
        return _theI2CBus.ping() == ksfTkErrOk;
    }

  private:
    sfTkArdI2C _theI2CBus;
};

//-----------------------------------------------------------------------------------------------
// Define our Arduino Object - SPI version
class SparkFunSoilMoistureSensorSPI : public sfDevSoilMoisture
{
  public:
    /**
     * @brief Begins the Device with SPI as the communication bus
     *
     * Initializes the soil moisture sensor for SPI communication using the specified
     * chip select pin, SPI port, and SPI settings.
     *
     * @param csPin The chip select pin for the sensor
     * @param spiPort The SPI port to use for communication (default is SPI)
     * @param spiSettings The SPI settings to use for communication (default is SPISettings(100000, MSBFIRST,
     * SPI_MODE0))
     * @return true if successful, false otherwise
     */
    bool begin(const uint8_t csPin, SPIClass &spiPort = SPI,
               SPISettings spiSettings = SPISettings(100000, MSBFIRST, SPI_MODE0))
    {

        // Setup Arduino SPI bus
        _theSPIBus.init(spiPort, spiSettings, csPin, true);

        // Begin the sensor with the SPI bus connection
        return (sfDevSoilMoisture::begin(&_theSPIBus) == ksfTkErrOk);
    }

  private:
    sfTkArdSPI _theSPIBus;
};