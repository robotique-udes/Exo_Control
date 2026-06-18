/**
 * @file CubemarsMotorV3.cpp
 * @brief Definition of the CubemarsMotorV3 implementation class
 * 
 * @author Samuel Savaria
 * @date 2026-06-06
 */
#include "CubemarsMotorV3.hpp"
#include <cmath> // For M_PI
constexpr const char* CubemarsMotorV3::ERROR_DESCRIPTIONS[]; // Avoids linker error with C++14 and below

CubemarsMotorV3::CubemarsMotorV3(uint8_t id) : ICubemarsMotor(id) {}

void CubemarsMotorV3::enterMode() 
{
    // Reset error flag
    m_errorCode = 0;
}

void CubemarsMotorV3::sendCommand(float p_position, float p_velocity, float p_torque, float p_kp, float p_kd)
{
    // Refer to section 4.2 of the datasheet

    // Convert floats to unsigned ints
    uint32_t positionInt = float_to_uint(p_position, POSITION_MIN, POSITION_MAX, 16);
    uint32_t velocityInt = float_to_uint(p_velocity, VELOCITY_MIN, VELOCITY_MAX, 12);
    uint32_t torqueInt = float_to_uint(p_torque, TORQUE_MIN, TORQUE_MAX, 12);
    uint32_t kpInt = float_to_uint(p_kp, KP_MIN, KP_MAX, 12);
    uint32_t kdInt = float_to_uint(p_kd, KD_MIN, KD_MAX, 12);

    // Pack ints to the buffer
    uint8_t buffer[8];
    buffer[0] = kpInt >> 8;
    buffer[1] = ((kpInt & 0xF) << 4) | (kdInt >> 8);
    buffer[2] = kdInt >> 4;
    buffer[3] = positionInt >> 8;
    buffer[4] = positionInt & 0xFF;
    buffer[5] = velocityInt >> 4;
    buffer[6] = ((velocityInt & 0xF) << 4) | (torqueInt >> 8);
    buffer[7] = torqueInt & 0xFF;

    // Send CAN message
    CanFrame message;
    message.identifier = m_motorId | FORCE_CONTROL_MODE;
    message.extd = 1;         //=========================================//
    message.rtr = 0;          //= The bit field is not zero initialized =//
    message.ss = 1;           //= Each field has to be set to avoid     =//
    message.self = 0;         //= garbage values                        =//
    message.dlc_non_comp = 0; //==========================================//
    message.data_length_code = sizeof(buffer);
    for(int i = 0; i < message.data_length_code; ++i)
    {
        message.data[i] = buffer[i];
    }

    ESP32Can.writeFrame(message);
}

void CubemarsMotorV3::receiveCommand(const CanFrame& p_message)
{
    // Refer to section 4.3.1 of the datasheet

    // Ignore the message if this motor is not the target
    if(p_message.identifier != (m_motorId | REPLY_MESSAGE_CODE)) return;

    // Parse the data into ints
    int16_t posInt = p_message.data[0] << 8 | p_message.data[1];
    int16_t spdInt = p_message.data[2] << 8 | p_message.data[3];
    int16_t trqInt = p_message.data[4] << 8 | p_message.data[5];
    int8_t tempInt = p_message.data[6];
    uint8_t errInt = p_message.data[7];

    // Save the data
    m_position = posInt * 0.1f * (M_PI / 180.0f); // Convert position to radians from degrees
    m_speed    = spdInt * 10.0f;
    m_torque  = trqInt * 0.01f;
    m_temperature = tempInt;
    if(errInt != 0)
    {
        m_errorCode = errInt;
    }
}

const char* CubemarsMotorV3::getErrorDescription() const
{
    if(m_errorCode < ERROR_COUNT)
    {
        return ERROR_DESCRIPTIONS[m_errorCode];
    }
    else
    {
        return "UnknownError";
    }
}