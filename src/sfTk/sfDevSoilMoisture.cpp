/**
 * @file sfDevSoilMoisture.cpp
 * @brief Implementation file for the soil moisture sensor class
 *
 * This file contains the implementation of the soil moisture sensor class, including
 * methods for initialization, reading moisture values, controlling the on-board LED,
 * and setting the I2C address.
 *
 * @author SparkFun Electronics
 * @date 2025
 * @copyright Copyright (c) 2025, SparkFun Electronics Inc. This project is released under the MIT License.
 * 
 * SPDX-License-Identifier: MIT
 */


#include "sfDevSoilMoisture.h"

// Impl for the core driver

// Define the communication commands for the soil moisture sensor (from the original zio firmware)
/**
 * @brief Command to turn off the on-board LED
 */
#define kCommandLEDOff 0x00

/**
 * @brief Command to turn on the on-board LED
 */
#define kCommandLEDOn 0x01

/**
 * @brief Command to change the I2C address of the sensor
 */
#define kCommandChangeAddress 0x03

/**
 * @brief Command to get the moisture value from the sensor
 */
#define kCommandGetValue 0x05

/**
 * @brief Command indicating no new data is available
 */
#define kCommandNothingNew 0x99

//---------------------------------------------------------------------
// Core object implementation
//---------------------------------------------------------------------
// start up the sensor
sfTkError_t sfDevSoilMoisture::begin(sfTkIBus *theBus)
{
    // Nullptr check
    if (theBus == nullptr)
        return ksfTkErrBusNotInit;

    // Set bus pointer
    _theBus = theBus;

    return ksfTkErrOk;
}

//----------------------------------------------------------------------------------------
// LED off command
sfTkError_t sfDevSoilMoisture::LEDOff(void)
{
    if (_theBus == nullptr)
        return ksfTkErrBusNotInit;

    // Send the command to turn the LED off
    return _theBus->writeByte(kCommandLEDOff);
}
//----------------------------------------------------------------------------------------
// LED on command
sfTkError_t sfDevSoilMoisture::LEDOn(void)
{
    if (_theBus == nullptr)
        return ksfTkErrBusNotInit;

    // Send the command to turn the LED on
    return _theBus->writeByte(kCommandLEDOn);
}

//----------------------------------------------------------------------------------------
// Read the moisture value from the sensor - returns a resistance reading between 0 and 1023
uint16_t sfDevSoilMoisture::readMoistureValue(void)
{
    if (_theBus == nullptr)
        return 0;

    uint16_t value = 0;
    if (_theBus->readRegisterWord(kCommandGetValue, value) != ksfTkErrOk)
        return 0;

    return value;
}

//----------------------------------------------------------------------------------------
// Returns the moisture ratio from the sensor (0 - 1.0)
float sfDevSoilMoisture::readMoistureRatio(void)
{
    if (_theBus == nullptr)
        return 0.0;

    return (((float)SF_SOIL_MOISTURE_MAX_VALUE - (float)readMoistureValue()) / (float)SF_SOIL_MOISTURE_MAX_VALUE);
}

//----------------------------------------------------------------------------------------
// Returns the moisture percentage from the sensor (0 - 100%)
float sfDevSoilMoisture::readMoisturePercentage(void)
{
    return readMoistureRatio() * 100.0;
}
//----------------------------------------------------------------------------------------
// Change the I2C address of the sensor
sfTkError_t sfDevSoilMoisture::setI2CAddress(uint8_t newAddress)
{
    if (_theBus == nullptr)
        return ksfTkErrBusNotInit;

    // Validate the new address
    if (newAddress < 0x07 || newAddress > 0x78)
        return ksfTkErrFail;

    // If in I2C mode, is the address the same as the current address?
    if (_theBus->type() == ksfTkBusTypeI2C && ((sfTkII2C *)_theBus)->address() == newAddress)
        return ksfTkErrOk;

    // Send the command to change the address. NOTE: Because of how the sensor works,
    // the following will return an error (since the sensor side resets the bus)
    (void)_theBus->writeRegisterByte(kCommandChangeAddress, newAddress);

    return ksfTkErrOk;
}
//----------------------------------------------------------------------------------------
// Return the address of the sensor bus. For I2C this is the address of the sensor, for
// SPI this is the CS pin
uint8_t sfDevSoilMoisture::address(void)
{
    if (_theBus == nullptr)
        return 0;

    if (_theBus->type() == ksfTkBusTypeSPI)
        return ((sfTkISPI *)_theBus)->cs();
    else if (_theBus->type() == ksfTkBusTypeI2C)
        return ((sfTkII2C *)_theBus)->address();

    return 0;
}
