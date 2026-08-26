/**
 * @file ICubemarsMotor.cpp
 * @brief Definition of the ICubemarsMotor interface class non-virtual functions
 * 
 * @author Samuel Savaria, Eloi Charbonneau, Gabriel Desrochers
 * @date 2026-06-05
 */
#include "ICubemarsMotor.hpp"

//#define DEBUG_INTERFACE_MOTOR // uncomment to display prints in this file
#ifdef DEBUG_INTERFACE_MOTOR
#include "prints/printMacro.hpp"
#else 
#include "prints/fakePrintMacro.hpp"
#endif

// This is necessary to avoid linker errors with C++14 and below
// I am not sure why this specific member is a problem
constexpr const char* ICubemarsMotor::ERROR_DESCRIPTIONS[ICubemarsMotor::MAX_ERROR];

ICubemarsMotor::ICubemarsMotor(uint8_t p_id)
{
    m_motorId = p_id;
}

uint8_t ICubemarsMotor::getMotorID() const
{
    return m_motorId;
}

float ICubemarsMotor::getPosition() const
{
    return m_position;
}

float ICubemarsMotor::getSpeed() const
{
    return m_speed;
}

float ICubemarsMotor::getTorque() const
{
    return m_torque;
}

int8_t ICubemarsMotor::getMosfetTemperature() const
{
    return m_temperature;
}

CubemarsErrorCode ICubemarsMotor::getErrorCode() const
{
    return m_errorCode;
}

const char* ICubemarsMotor::getErrorDescription() const
{
    return ERROR_DESCRIPTIONS[static_cast<uint8_t>(m_errorCode)];
}

uint32_t ICubemarsMotor::float_to_uint(float p_value, float p_min, float p_max, uint8_t p_bits)
{
    // This is CubeMars code, please don't judge me
    float span = p_max - p_min; 
    if(p_value < p_min) p_value = p_min; 
    else if(p_value > p_max) p_value = p_max; 
    return (uint32_t) ((p_value - p_min)*((float)((1<<p_bits)/span)));
}

float ICubemarsMotor::uint_to_float(uint32_t p_value, float p_min, float p_max, uint8_t p_bits)
{
    // This is CubeMars code, please don't judge me
    float span = p_max - p_min; 
    float offset = p_min; 
    return ((float)p_value)*span/((float)((1<<p_bits)-1)) + offset; 
}