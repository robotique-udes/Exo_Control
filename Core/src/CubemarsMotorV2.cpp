/**
 * @file CubemarsMotorV2.cpp
 * @brief Definition of the CubemarsMotorV2 implementation class
 *
 * @author Samuel Savaria
 * @date 2026-06-07
 */
#include "CubemarsMotorV2.hpp"

CubemarsMotorV2::CubemarsMotorV2(uint8_t p_id) : ICubemarsMotor(p_id) {}

void CubemarsMotorV2::enterMode()
{
    // Refer to section 5.3 of the datasheet

    // Enter Mode command
    CanFrame enterModeMessage;

    enterModeMessage.identifier = m_motorId; 
    enterModeMessage.extd = 0;             //=========================================//
    enterModeMessage.rtr = 0;              //= The bit field is not zero initialized =//
    enterModeMessage.ss = 1;               //= Each field has to be set to avoid     =//
    enterModeMessage.self = 0;             //= garbage values                        =//
    enterModeMessage.dlc_non_comp = 0;     //==========================================//

    enterModeMessage.data_length_code = 8;
    enterModeMessage.data[0] = 0xFF;
    enterModeMessage.data[1] = 0xFF;
    enterModeMessage.data[2] = 0xFF;
    enterModeMessage.data[3] = 0xFF;
    enterModeMessage.data[4] = 0xFF;
    enterModeMessage.data[5] = 0xFF;
    enterModeMessage.data[6] = 0xFF;
    enterModeMessage.data[7] = 0xFC;

    ESP32Can.writeFrame(enterModeMessage);

    // Zero Set command
    CanFrame zeroSetMessage;

    zeroSetMessage.identifier = m_motorId; 
    zeroSetMessage.extd = 0;             //=========================================//
    zeroSetMessage.rtr = 0;              //= The bit field is not zero initialized =//
    zeroSetMessage.ss = 1;               //= Each field has to be set to avoid     =//
    zeroSetMessage.self = 0;             //= garbage values                        =//
    zeroSetMessage.dlc_non_comp = 0;     //==========================================//

    zeroSetMessage.data_length_code = 8;
    zeroSetMessage.data[0] = 0xFF;
    zeroSetMessage.data[1] = 0xFF;
    zeroSetMessage.data[2] = 0xFF;
    zeroSetMessage.data[3] = 0xFF;
    zeroSetMessage.data[4] = 0xFF;
    zeroSetMessage.data[5] = 0xFF;
    zeroSetMessage.data[6] = 0xFF;
    zeroSetMessage.data[7] = 0xFE;
    
    ESP32Can.writeFrame(zeroSetMessage);

    // Reset error flag
    m_errorCode = CubemarsErrorCode::NO_FAULT;
}

void CubemarsMotorV2::sendCommand(float p_position, float p_velocity, float p_torque, float p_kp, float p_kd)
{
    // Refer to section 5.3 of the datasheet

    // Convert floats to unsigned ints
    uint32_t positionInt = float_to_uint(p_position, POSITION_MIN, POSITION_MAX, 16);
    uint32_t velocityInt = float_to_uint(p_velocity, VELOCITY_MIN, VELOCITY_MAX, 12);
    uint32_t torqueInt = float_to_uint(p_torque, TORQUE_MIN, TORQUE_MAX, 12);
    uint32_t kpInt = float_to_uint(p_kp, KP_MIN, KP_MAX, 12);
    uint32_t kdInt = float_to_uint(p_kd, KD_MIN, KD_MAX, 12);

    // Pack ints to the buffer
    uint8_t buffer[8];
    buffer[0] = positionInt >> 8;
    buffer[1] = positionInt & 0xFF;
    buffer[2] = velocityInt >> 4;
    buffer[3] = ((velocityInt & 0xF) << 4) | (kpInt >> 8);
    buffer[4] = kpInt & 0xFF;
    buffer[5] = kdInt >> 4;
    buffer[6] = ((kdInt & 0xF) << 4) | (torqueInt >> 8);
    buffer[7] = torqueInt & 0xFF;

    // Send CAN message
    CanFrame message;
    message.identifier = m_motorId;
    message.extd = 0;         //=========================================//
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

void CubemarsMotorV2::receiveCommand(const CanFrame& p_message)
{
    // Refer to section 5.3 of the datasheet

    // Ignore the message if this motor is not the target
    if(p_message.identifier != m_motorId) return;

    // Parse the data into ints
    uint16_t posInt = p_message.data[1] << 8 | p_message.data[2];
    uint16_t spdInt = p_message.data[3] << 4 | p_message.data[4] >> 4;
    uint16_t trqInt = (p_message.data[4] & 0xF) << 8 | p_message.data[5];
    uint8_t tempInt = p_message.data[6];
    uint8_t errInt = p_message.data[7];
    
    // Save the data
    m_position = uint_to_float(posInt, POSITION_MIN, POSITION_MAX, 16);
    m_speed = uint_to_float(spdInt, VELOCITY_MIN, VELOCITY_MAX, 12); 
    m_torque = uint_to_float(trqInt, TORQUE_MIN, TORQUE_MAX, 12); 
    m_temperature = (int)tempInt - 40;
    if(errInt != 0)
    {
        m_errorCode = static_cast<CubemarsErrorCode>(errInt);
    }
}