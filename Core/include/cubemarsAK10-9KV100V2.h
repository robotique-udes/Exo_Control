/**
 * @file cubemarsAK10-9KV100V2.h
 * @brief Declaration of the Cubemars_AK10_9_KV100_V2 implementation class
 * @see Datasheet: https://www.cubemars.com/data/cms/202604/ak-series-driver-manual-v1-0-18-for-ak-2-0-robotic-actuator.pdf
 * 
 * @author Samuel Savaria
 * @date 2026-06-07
 */
#ifndef BIOGENIUS_CUBEMARS_AK10_KV10_9_KV100_V2_H
#define BIOGENIUS_CUBEMARS_AK10_KV10_9_KV100_V2_H

#include "mitModeMotor.h"

/**
 * @brief Implementation of the IMitModeMotor interface for a CubeMars AK10-9 KV100 V2 motor
 *
 * @details The class contains the parameter range for the AK10-9 KV100 V2 model
 *          and implements the interface's pure virtual functions
 *
 * @author Samuel Savaria
 * @date 2026-06-07
 */
class Cubemars_AK10_9_KV100_V2 : public IMitModeMotor
{
private:
    static constexpr inline float POSITION_MIN = -12.5f;
    static constexpr inline float POSITION_MAX =  12.5f;
    static constexpr inline float VELOCITY_MIN = -50.0f;
    static constexpr inline float VELOCITY_MAX =  50.0f;
    static constexpr inline float TORQUE_MIN   = -65.0f;
    static constexpr inline float TORQUE_MAX   =  65.0f;
    static constexpr inline float KP_MIN       =  0.0f;
    static constexpr inline float KP_MAX       =  500.0f;
    static constexpr inline float KD_MIN       =  0.0f;
    static constexpr inline float KD_MAX       =  5.0f;

public:
/**
     * @brief Constructor
     *
     * @param id The 8-bit CAN ID of the motor
     */
    Cubemars_AK10_9_KV100_V2(uint8_t id);

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