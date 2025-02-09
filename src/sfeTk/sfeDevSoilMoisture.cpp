
/*
 *---------------------------------------------------------------------------------
 *
 * Copyright (c) 2025, SparkFun Electronics Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 *---------------------------------------------------------------------------------
 */
#include "sfeDevSoilMoisture.h"

// Impl for the core driver

// Define the communication commands for the soil moisture sensor (from the original zio firmware)
#define kCommandLEDOff 0x00
#define kCommandLEDOn 0x01
#define kCommandChangeAddress 0x03
#define kCommandGetValue 0x05
#define kCommandNothingNew 0x99
//---------------------------------------------------------------------
// Core object implementation
//---------------------------------------------------------------------
sfeTkError_t sfeDevSoilMoisture::begin(sfeTkII2C *theBus)
{
    // Nullptr check
    if (theBus == nullptr)
        return kSTkErrFail;

    // Set bus pointer
    _theBus = theBus;

    return _theBus->ping();
}

//----------------------------------------------------------------------------------------
sfeTkError_t sfeDevSoilMoisture::LEDOff(void)
{
    if (_theBus == nullptr)
        return kSTkErrFail;

    // Send the command to turn the LED off
    return _theBus->writeByte(kCommandLEDOff);
}
//----------------------------------------------------------------------------------------
sfeTkError_t sfeDevSoilMoisture::LEDOn(void)
{
    if (_theBus == nullptr)
        return kSTkErrFail;

    // Send the command to turn the LED on
    return _theBus->writeByte(kCommandLEDOn);
}

//----------------------------------------------------------------------------------------
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
sfeTkError_t sfeDevSoilMoisture::changeSensorAddress(uint8_t newAddress)
{
    if (_theBus == nullptr)
        return kSTkErrFail;

    // Validate the new address
    if (newAddress < 0x07 || newAddress > 0x78)
        return kSTkErrFail;

    // Send the command to change the address
    return _theBus->writeRegisterByte(kCommandChangeAddress, newAddress);
}
