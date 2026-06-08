/**
 * @file cubemarsAK10-9KV60V3.h
 * @brief Declaration of the Cubemars_AK10_9_KV60_V3 implementation class
 * @see Datasheet: https://www.cubemars.com/images/file/20241217/1734428582786471.pdf
 * 
 * @author Samuel Savaria
 * @date 2026-06-06
 */
#ifndef BIOGENIUS_CUBEMARS_AK10_KV10_9_KV60_V3_H
#define BIOGENIUS_CUBEMARS_AK10_KV10_9_KV60_V3_H

#include "mitModeMotor.h"

/**
 * @brief Implementation of the IMitModeMotor interface for a CubeMars AK10-9 KV60 V3 motor
 *
 * @details The class contains the parameter range for the AK10-9 KV60 V3 model
 *          and implements the interface's pure virtual functions
 *
 * @author Samuel Savaria
 * @date 2026-06-06
 */
class Cubemars_AK10_9_KV60_V3 : public IMitModeMotor
{
private:
    static constexpr inline float POSITION_MIN = -12.56f;
    static constexpr inline float POSITION_MAX =  12.56f;
    static constexpr inline float VELOCITY_MIN = -28.0f;
    static constexpr inline float VELOCITY_MAX =  28.0f;
    static constexpr inline float TORQUE_MIN   = -54.0f;
    static constexpr inline float TORQUE_MAX   =  54.0f;
    static constexpr inline float KP_MIN       =  0.0f;
    static constexpr inline float KP_MAX       =  500.0f;
    static constexpr inline float KD_MIN       =  0.0f;
    static constexpr inline float KD_MAX       =  5.0f;
    static constexpr inline uint32_t FORCE_CONTROL_MODE = 0x800;
    static constexpr inline uint32_t REPLY_MESSAGE_CODE = 0x2900;

public:
/**
     * @brief Constructor
     *
     * @param id The 8-bit CAN ID of the motor
     */
    Cubemars_AK10_9_KV60_V3(uint8_t id);

    /**
     * @brief Enter MIT mode
     */
    void enterMode() override;

    /**
     * @brief Sends a MIT mode command
     *
     * @param position The target position in radians. Also requires KP and KD
     * @param velocity The target velocity in radians/second. Also requires KD
     * @param torque   The target torque in newtons-meter.
     * @param kp       The proportional gain. Required by position
     * @param kd       The derivative gain. Required by position and velocity
     */
    void sendCommand(float position, float velocity, float torque, float kp, float kd) override;

    /**
     * @brief Parses a CAN message sent by the motor containing the position, speed, torque, temperature and error code
     *
     * @param message The CAN message to parse
     */
    void receiveCommand(const CanFrame& message) override;
};

#endif