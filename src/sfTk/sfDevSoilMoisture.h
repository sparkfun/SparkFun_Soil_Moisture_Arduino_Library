/**
 * @file sfDevSoilMoisture.h
 * @brief Header file for the soil moisture sensor class
 *
 * This file contains the class definition for the soil moisture sensor, including
 * methods for initialization, reading moisture values, controlling the on-board LED,
 * and setting the I2C address.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 *
 * SPDX-License-Identifier: MIT
 */

// Actual implementation of the soil moisture sensor - this is a platform "independent" implementation

#pragma once

#include <stdint.h>

// include the sparkfun toolkit headers
#include <sfTk/sfToolkit.h>

// Bus interfaces
#include <sfTk/sfTkII2C.h>
#include <sfTk/sfTkISPI.h>

/**
 * @brief Default I2C address for the soil moisture sensor
 *
 * This macro defines the default I2C address (0x28) used by the soil moisture sensor.
 */
#define SF_SOIL_MOISTURE_DEFAULT_I2C_ADDRESS 0x28

/**
 * @brief Maximum value for the soil moisture sensor
 *
 * This macro defines the maximum value (1023) for the soil moisture sensor,
 * which corresponds to the highest reading from the 10-bit ADC.  2^10  = 1024-1
 */
#define SF_SOIL_MOISTURE_MAX_VALUE 1023

/**
 * @brief Class representing the soil moisture sensor
 *
 * This class provides an interface to the soil moisture sensor, allowing for
 * initialization, reading moisture values and controlling the on-board LED.
 */
class sfDevSoilMoisture
{

  public:
    // ctor
    sfDevSoilMoisture() : _theBus{nullptr}
    {
    }

    /**
     * @brief Initializes communication with the soil moisture sensor
     *
     * This function must be called before any other methods. It establishes
     * communication with the sensor using the provided I2C bus interface.
     *
     * @param theBus Pointer to an I2C toolkit object. If nullptr, uses previously set bus
     * @return ksfTkErrOk if initialization succeeds
     * @return ksfTkErrBusNotInit if no valid bus interface is provided
     */
    sfTkError_t begin(sfTkIBus *theBus = nullptr);

    /**
     * @brief Turns off the on-board LED
     *
     * Disables the sensor's built-in LED
     *
     * @return ksfTkErrOk if LED was successfully turned off
     * @return ksfTkErrBusNotInit if no bus interface is configured
     * @return ksfTkErrFail if sensor doesn't respond or a communication error occurs
     *
     * @see LEDOn()
     */
    sfTkError_t LEDOff(void);

    /**
     * @brief Turns on the on-board LED
     *
     * Enables the sensor's built-in LED by sending the appropriate command
     *
     * @return ksfTkErrOk if LED was successfully turned on
     * @return ksfTkErrBusNotInit if no bus interface is configured
     * @return ksfTkErrFail if sensor doesn't respond or a communication error occurs
     *
     * @see LEDOff()
     */
    sfTkError_t LEDOn(void);

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
     * @return ksfTkErrOk if successful, otherwise an error code
     * @note If communicating via I2C, the provided address is used for future I2C communication
     */
    sfTkError_t setI2CAddress(uint8_t newAddress);

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
    /**
     * @brief The toolkit bus the sensor is connected to
     *
     * This member variable holds a pointer to the bus interface (I2C or SPI) used
     * for communication with the soil moisture sensor.
     */
    sfTkIBus *_theBus;

}; // class sfDevSoilMoisture