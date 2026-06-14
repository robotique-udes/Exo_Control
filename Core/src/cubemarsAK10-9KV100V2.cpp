/**
 * @file cubemarsAK10-9KV100V2.cpp
 * @brief Definition of the Cubemars_AK10_9_KV100_V2 implementation class
 *
 * @author Samuel Savaria
 * @date 2026-06-07
 */
#include "cubemarsAK10-9KV100V2.h"
constexpr const char* Cubemars_AK10_9_KV100_V2::ERROR_DESCRIPTIONS[]; // Avoids linker error with C++14 and below

Cubemars_AK10_9_KV100_V2::Cubemars_AK10_9_KV100_V2(uint8_t id) : IMitModeMotor(id) {}

void Cubemars_AK10_9_KV100_V2::enterMode()
{
    // Refer to section 5.3 of the datasheet

    // Create enter mode message
    CanFrame enterModeMessage;

    enterModeMessage.identifier = motorID; 
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

    // Create zero set message
    CanFrame zeroSetMessage;

    zeroSetMessage.identifier = motorID; 
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
    
    // Send messages
    ESP32Can.writeFrame(enterModeMessage);
    ESP32Can.writeFrame(zeroSetMessage);\

    // Reset error flag
    errorCode = 0;
}

void Cubemars_AK10_9_KV100_V2::sendCommand(float position, float velocity, float torque, float kp, float kd)
{
    // Refer to section 5.3 of the datasheet

    // Convert floats to unsigned ints
    uint32_t position_int = float_to_uint(position, POSITION_MIN, POSITION_MAX, 16);
    uint32_t velocity_int = float_to_uint(velocity, VELOCITY_MIN, VELOCITY_MAX, 12);
    uint32_t torque_int = float_to_uint(torque, TORQUE_MIN, TORQUE_MAX, 12);
    uint32_t kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    uint32_t kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);

    // Pack ints to the buffer
    uint8_t buffer[8];
    buffer[0] = position_int >> 8;
    buffer[1] = position_int & 0xFF;
    buffer[2] = velocity_int >> 4;
    buffer[3] = ((velocity_int & 0xF) << 4) | (kp_int >> 8);
    buffer[4] = kp_int & 0xFF;
    buffer[5] = kd_int >> 4;
    buffer[6] = ((kd_int & 0xF) << 4) | (torque_int >> 8);
    buffer[7] = torque_int & 0xFF;

    // Send CAN message
    CanFrame message;
    message.identifier = motorID;
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

void Cubemars_AK10_9_KV100_V2::receiveCommand(const CanFrame& message)
{
    // Refer to section 5.3 of the datasheet

    // Ignore the message if this motor is not the target
    if(message.identifier != motorID) return;

    // Parse the data into ints
    uint16_t pos_int = message.data[1] << 8 | message.data[2];
    uint16_t spd_int = message.data[3] << 4 | message.data[4] >> 4;
    uint16_t trq_int = (message.data[4] & 0xF) << 8 | message.data[5];
    uint8_t temp_int = message.data[6];
    uint8_t err_int = message.data[7];
    
    // Save the data
    position = uint_to_float(pos_int, POSITION_MIN, POSITION_MAX, 16);
    speed = uint_to_float(spd_int, VELOCITY_MIN, VELOCITY_MAX, 12); 
    torque = uint_to_float(trq_int, TORQUE_MIN, TORQUE_MAX, 12); 
    temperature = (int)temp_int - 40;
    if(err_int != 0)
    {
        errorCode = err_int;
    }
}

const char* Cubemars_AK10_9_KV100_V2::getErrorDescription() const
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