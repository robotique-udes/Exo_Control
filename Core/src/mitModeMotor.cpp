/**
 * @file MitModeMotor.cpp
 * @brief Definition of the IMitModeMotor interface class non-virtual functions
 * 
 * @author Samuel Savaria
 * @date 2026-06-05
 */
#include "MitModeMotor.h"

IMitModeMotor::IMitModeMotor(uint8_t p_id)
{
    m_motorId = p_id;
}

uint8_t IMitModeMotor::getMotorID() const
{
    return m_motorId;
}

float IMitModeMotor::getPosition() const
{
    return m_position;
}

float IMitModeMotor::getSpeed() const
{
    return m_speed;
}

float IMitModeMotor::getTorque() const
{
    return m_torque;
}

int8_t IMitModeMotor::getMosfetTemperature() const
{
    return m_temperature;
}

uint8_t IMitModeMotor::getErrorCode() const
{
    return m_errorCode;
}

uint32_t IMitModeMotor::float_to_uint(float p_value, float p_min, float p_max, uint8_t p_bits)
{
    // This is CubeMars code, please don't judge me
    float span = p_max - p_min; 
    if(p_value < p_min) p_value = p_min; 
    else if(p_value > p_max) p_value = p_max; 
    return (uint32_t) ((p_value - p_min)*((float)((1<<p_bits)/span)));
}

float IMitModeMotor::uint_to_float(uint32_t p_value, float p_min, float p_max, uint8_t p_bits)
{
    // This is CubeMars code, please don't judge me
    float span = p_max - p_min; 
    float offset = p_min; 
    return ((float)p_value)*span/((float)((1<<p_bits)-1)) + offset; 
}