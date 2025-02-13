/**
 * @file sfeDevSoilMoisture.cpp
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


#include "sfeDevSoilMoisture.h"

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
sfeTkError_t sfeDevSoilMoisture::begin(sfeTkIBus *theBus)
{
    // Nullptr check
    if (theBus == nullptr)
        return kSTkErrBusNotInit;

    // Set bus pointer
    _theBus = theBus;

    return kSTkErrOk;
}

//----------------------------------------------------------------------------------------
// LED off command
sfeTkError_t sfeDevSoilMoisture::LEDOff(void)
{
    if (_theBus == nullptr)
        return kSTkErrBusNotInit;

    // Send the command to turn the LED off
    return _theBus->writeByte(kCommandLEDOff);
}
//----------------------------------------------------------------------------------------
// LED on command
sfeTkError_t sfeDevSoilMoisture::LEDOn(void)
{
    if (_theBus == nullptr)
        return kSTkErrBusNotInit;

    // Send the command to turn the LED on
    return _theBus->writeByte(kCommandLEDOn);
}

//----------------------------------------------------------------------------------------
// Read the moisture value from the sensor - returns a resistance reading between 0 and 1023
uint16_t sfeDevSoilMoisture::readMoistureValue(void)
{
    if (_theBus == nullptr)
        return 0;

    uint16_t value = 0;
    if (_theBus->readRegisterWord(kCommandGetValue, value) != kSTkErrOk)
        return 0;

    return value;
}

//----------------------------------------------------------------------------------------
// Returns the moisture ratio from the sensor (0 - 1.0)
float sfeDevSoilMoisture::readMoistureRatio(void)
{
    if (_theBus == nullptr)
        return 0.0;

    return (((float)SFE_SOIL_MOISTURE_MAX_VALUE - (float)readMoistureValue()) / (float)SFE_SOIL_MOISTURE_MAX_VALUE);
}

//----------------------------------------------------------------------------------------
// Returns the moisture percentage from the sensor (0 - 100%)
float sfeDevSoilMoisture::readMoisturePercentage(void)
{
    return readMoistureRatio() * 100.0;
}
//----------------------------------------------------------------------------------------
// Change the I2C address of the sensor
sfeTkError_t sfeDevSoilMoisture::setI2CAddress(uint8_t newAddress)
{
    if (_theBus == nullptr)
        return kSTkErrBusNotInit;

    // Validate the new address
    if (newAddress < 0x07 || newAddress > 0x78)
        return kSTkErrFail;

    // If in I2C mode, is the address the same as the current address?
    if (_theBus->type() == kBusTypeI2C && ((sfeTkII2C *)_theBus)->address() == newAddress)
        return kSTkErrOk;

    // Send the command to change the address. NOTE: Because of how the sensor works,
    // the following will return an error (since the sensor side resets the bus)
    (void)_theBus->writeRegisterByte(kCommandChangeAddress, newAddress);

    return kSTkErrOk;
}
//----------------------------------------------------------------------------------------
// Return the address of the sensor bus. For I2C this is the address of the sensor, for
// SPI this is the CS pin
uint8_t sfeDevSoilMoisture::address(void)
{
    if (_theBus == nullptr)
        return 0;

    if (_theBus->type() == kBusTypeSPI)
        return ((sfeTkISPI *)_theBus)->cs();
    else if (_theBus->type() == kBusTypeI2C)
        return ((sfeTkII2C *)_theBus)->address();

    return 0;
}
