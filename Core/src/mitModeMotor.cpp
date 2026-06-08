/**
 * @file mitModeMotor.cpp
 * @brief Definition of the IMitModeMotor interface class non-virtual functions
 * 
 * @author Samuel Savaria
 * @date 2026-06-05
 */
#include "mitModeMotor.h"

IMitModeMotor::IMitModeMotor(uint8_t id)
{
    motorID = id;
}

uint8_t IMitModeMotor::getMotorID() const
{
    return motorID;
}

float IMitModeMotor::getPosition() const
{
    return position;
}

float IMitModeMotor::getSpeed() const
{
    return speed;
}

float IMitModeMotor::getTorque() const
{
    return torque;
}

int8_t IMitModeMotor::getTemperature() const
{
    return temperature;
}

CubeMarsErrorCode IMitModeMotor::getErrorCode() const
{
    return errorCode;
}

uint32_t IMitModeMotor::float_to_uint(float value, float min, float max, uint8_t bits)
{
    // This is CubeMars code, please don't judge me
    float span = max - min; 
    if(value < min) value = min; 
    else if(value > max) value = max; 
    return (uint32_t) ((value - min)*((float)((1<<bits)/span)));
}

float IMitModeMotor::uint_to_float(uint32_t value, float min, float max, uint8_t bits)
{
    // This is CubeMars code, please don't judge me
    float span = max - min; 
    float offset = min; 
    return ((float)value)*span/((float)((1<<bits)-1)) + offset; 
}