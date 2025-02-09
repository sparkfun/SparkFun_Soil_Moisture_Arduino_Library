/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */
// Actual implementation def of the soil moisture sensor

#pragma once

#include <cstdint>
#include "sfeTk/sfeTkII2C.h"
#include <sfeTk/sfeToolKit.h>

// Default I2C address for the sensor
#define SFE_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS 0x28

// Max value for  the sensor value (10-bit ADC) 2^10  = 1024-1
#define SFE_SOIL_MOISTURE_MAX_VALUE 1023

class sfeDevSoilMoisture
{

  public:
    // ctor
    sfeDevSoilMoisture() : _theBus{nullptr}
    {
    }

    /// @brief Start the driver/begin connecting/comms to the device
    /// @param theBus A pointer to an I2C toolkit object
    /// @return kSTkErrOk if successful, otherwise an error code
    sfeTkError_t begin(sfeTkII2C *theBus = nullptr);

    /// @brief Turns off the on-board LED
    /// @return kSTkErrOk if successful, otherwise an error code
    sfeTkError_t LEDOff(void);

    /// @brief Turns on the on-board LED
    /// @return kSTkErrOk if successful, otherwise an error code    
    sfeTkError_t LEDOn(void);

    /// @brief Reads the moisture value from the sensor
    /// @return The moisture value - a resistance reading between 0 and 1023
    uint16_t readMoistureValue(void);

    /// @brief Reads the sensor value and returns the moisture ratio from the sensor (0 - 1.0)
    /// @return The moisture ratio
    float readMoistureRatio(void);

    /// @brief Reads the sensor value and returns the moisture percentage from the sensor (0 - 100%)
    /// @return The moisture percentage
    float readMoisturePercentage(void);

    /// @brief Changes the I2C address of the sensor
    /// @param newAddress The new I2C address to assign to the sensor
    /// @return kSTkErrOk if successful, otherwise an error code
    sfeTkError_t changeSensorAddress(uint8_t newAddress);

  protected:
    // The I2C bus the sensor is connected to
    sfeTkII2C *_theBus;

}; // class sfeDevSoilMoisture