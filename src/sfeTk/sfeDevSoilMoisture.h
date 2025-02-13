/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */
// Actual implementation of the soil moisture sensor - this is a platform "independent" implementation

#pragma once

#include <stdint.h>

// include the sparkfun toolkit headers
#include <sfeTk/sfeToolkit.h>

// Bus interfaces
#include <sfeTk/sfeTkII2C.h>
#include <sfeTk/sfeTkISPI.h>

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

    /**
     * @brief Initializes communication with the soil moisture sensor
     *
     * This function must be called before any other methods. It establishes
     * communication with the sensor using the provided I2C bus interface.
     *
     * @param theBus Pointer to an I2C toolkit object. If nullptr, uses previously set bus
     * @return kSTkErrOk if initialization succeeds
     * @return kSTkErrBusNotInit if no valid bus interface is provided
     */
    sfeTkError_t begin(sfeTkIBus *theBus = nullptr);

    /**
     * @brief Turns off the on-board LED
     *
     * Disables the sensor's built-in LED
     *
     * @return kSTkErrOk if LED was successfully turned off
     * @return kSTkErrBusNotInit if no bus interface is configured
     * @return kSTkErrFail if sensor doesn't respond or a communication error occurs
     *
     * @see LEDOn()
     */
    sfeTkError_t LEDOff(void);

    /**
     * @brief Turns on the on-board LED
     *
     * Enables the sensor's built-in LED by sending the appropriate command
     *
     * @return kSTkErrOk if LED was successfully turned on
     * @return kSTkErrBusNotInit if no bus interface is configured
     * @return kSTkErrFail if sensor doesn't respond or a communication error occurs
     *
     * @see LEDOff()
     */
    sfeTkError_t LEDOn(void);

    /**
     * @brief Reads the moisture value from the sensor
     *
     * This function reads the raw moisture value from the sensor, which is a
     * resistance reading between 0 and 1023.
     *
     * @return The moisture value - a resistance reading between 0 and 1023
     * @note A value of 0 indicates a wet sensor, while 1023 indicates a dry sensor
     * @note On error, this method returns 0
     */
    uint16_t readMoistureValue(void);

    /**
     * @brief Reads the sensor value and returns the moisture ratio from the sensor
     *
     * This function reads the raw moisture value from the sensor and converts it
     * to a ratio between 0 and 1.0.
     *
     * @return The moisture ratio, where 0 indicates a dry sensor and 1.0 indicates a wet sensor
     * @note On error, this method returns 0
     */
    float readMoistureRatio(void);

    /**
     * @brief Reads the sensor value and returns the moisture percentage from the sensor
     *
     * This function reads the raw moisture value from the sensor and converts it
     * to a percentage between 0 and 100%.
     *
     * @return The moisture percentage, where 0% indicates a dry sensor and 100% indicates a wet sensor
     */
    float readMoisturePercentage(void);

    /**
     * @brief Changes the I2C address of the sensor
     *
     * This function sets a new I2C address for the sensor. The new address will be used
     * for all future I2C communication with the sensor. This value is persistent
     *
     * @param newAddress The new I2C address to assign to the sensor
     * @return kSTkErrOk if successful, otherwise an error code
     * @note If communicating via I2C, the provided address is used for future I2C communication
     */
    sfeTkError_t setI2CAddress(uint8_t newAddress);

    /**
     * @brief Returns the current address of the sensor
     *
     * This function returns the current I2C address of the sensor if using I2C,
     * or the Chip Select (CS) pin if using SPI.
     *
     * @return The current I2C address of the sensor, or the CS pin for SPI
     */
    uint8_t address(void);

  protected:
    // The toolkit bus the sensor is connected to
    sfeTkIBus *_theBus;

}; // class sfeDevSoilMoisture