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

#define kSfeDevSoilMoistureDefaultI2CAddress 0x28

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

  protected:
    // The I2C bus the sensor is connected to
    sfeTkII2C *_theBus;

}; // class sfeDevSoilMoisture