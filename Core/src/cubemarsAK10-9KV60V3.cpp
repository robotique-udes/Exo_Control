/**
 * @file cubemarsAK10-9KV60V3.cpp
 * @brief Definition of the Cubemars_AK10_9_KV60_V3 implementation class
 * 
 * @author Samuel Savaria
 * @date 2026-06-06
 */
#include "cubemarsAK10-9KV60V3.h"
#include <cmath> // For M_PI
constexpr const char* Cubemars_AK10_9_KV60_V3::ERROR_DESCRIPTIONS[]; // Avoids linker error with C++14 and below

Cubemars_AK10_9_KV60_V3::Cubemars_AK10_9_KV60_V3(uint8_t id) : IMitModeMotor(id) {}

void Cubemars_AK10_9_KV60_V3::enterMode() 
{
    // Reset error flag
    errorCode = 0;
}

void Cubemars_AK10_9_KV60_V3::sendCommand(float position, float velocity, float torque, float kp, float kd)
{
    // Refer to section 4.2 of the datasheet

    // Convert floats to unsigned ints
    uint32_t position_int = float_to_uint(position, POSITION_MIN, POSITION_MAX, 16);
    uint32_t velocity_int = float_to_uint(velocity, VELOCITY_MIN, VELOCITY_MAX, 12);
    uint32_t torque_int = float_to_uint(torque, TORQUE_MIN, TORQUE_MAX, 12);
    uint32_t kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    uint32_t kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);

    // Pack ints to the buffer
    uint8_t buffer[8];
    buffer[0] = kp_int >> 8;
    buffer[1] = ((kp_int & 0xF) << 4) | (kd_int >> 8);
    buffer[2] = kd_int >> 4;
    buffer[3] = position_int >> 8;
    buffer[4] = position_int & 0xFF;
    buffer[5] = velocity_int >> 4;
    buffer[6] = ((velocity_int & 0xF) << 4) | (torque_int >> 8);
    buffer[7] = torque_int & 0xFF;

    // Send CAN message
    CanFrame message;
    message.identifier = motorID | FORCE_CONTROL_MODE;
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

void Cubemars_AK10_9_KV60_V3::receiveCommand(const CanFrame& message)
{
    // Refer to section 4.3.1 of the datasheet

    // Ignore the message if this motor is not the target
    if(message.identifier != (motorID | REPLY_MESSAGE_CODE)) return;

    // Parse the data into ints
    int16_t pos_int = message.data[0] << 8 | message.data[1];
    int16_t spd_int = message.data[2] << 8 | message.data[3];
    int16_t trq_int = message.data[4] << 8 | message.data[5];
    int8_t temp_int = message.data[6];
    uint8_t err_int = message.data[7];

    // Save the data
    position = pos_int * 0.1f * (M_PI / 180.0f); // Convert position to radians from degrees
    speed    = spd_int * 10.0f;
    torque  = trq_int * 0.01f;
    temperature = temp_int;
    if(err_int != 0)
    {
        errorCode = err_int;
    }
}

const char* Cubemars_AK10_9_KV60_V3::getErrorDescription() const
{
    if(errorCode < ERROR_COUNT)
    {
        return ERROR_DESCRIPTIONS[errorCode];
    }
    else
    {
        return "UnknownError";
    }
}