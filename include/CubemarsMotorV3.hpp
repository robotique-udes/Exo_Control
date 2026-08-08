/**
 * @file CubemarsMotorV3.hpp
 * @brief Declaration of the CubemarsMotorV3 implementation class
 * @see Datasheet: https://www.cubemars.com/images/file/20241217/1734428582786471.pdf
 * 
 * @author Samuel Savaria, Eloi Charbonneau, Gabriel Desrochers
 * @date 2026-06-06
 */
#ifndef BIOGENIUS_CUBEMARS_MOTOR_V3_HPP
#define BIOGENIUS_CUBEMARS_MOTOR_V3_HPP

#include "ICubemarsMotor.hpp"

/**
 * @brief Implementation of the ICubemarsMotor interface for a CubeMars V3 motor
 *
 * @warning The parameter range for the AK10-9 KV60 V3 model is hardcoded in this class
 *          If more V3 models were to be used, the parameters would need to be member variables instead of class attributes
 *
 * @author Samuel Savaria
 * @date 2026-06-06
 */
class CubemarsMotorV3 : public ICubemarsMotor
{
public:
/**
     * @brief Constructor
     *
     * @param[in] id The 8-bit CAN ID of the motor
     */
    CubemarsMotorV3(uint8_t id);

    /**
     * @brief Enter MIT mode
     */
    void enterMode() override;

    /**
     * @brief Sends a MIT mode command
     *
     * @param[in] position The target position in radians. Also requires KP and KD
     * @param[in] velocity The target velocity in radians/second. Also requires KD
     * @param[in] torque   The target torque in newtons-meter.
     * @param[in] kp       The proportional gain. Required by position
     * @param[in] kd       The derivative gain. Required by position and velocity
     */
    void sendCommand(float position, float velocity, float torque, float kp, float kd) override;

    /**
     * @brief Parses a CAN message sent by the motor containing the position, speed, torque, temperature and error code
     *
     * @param[in] message The CAN message to parse
     */
    void receiveCommand(const CanFrame& message) override;

private:
    // Refer to section 4.2 of the datasheet for the parameter range of the AK10-9
    static constexpr float POSITION_MIN = -12.56f;
    static constexpr float POSITION_MAX =  12.56f;
    static constexpr float VELOCITY_MIN = -28.0f;
    static constexpr float VELOCITY_MAX =  28.0f;
    static constexpr float TORQUE_MIN   = -54.0f;
    static constexpr float TORQUE_MAX   =  54.0f;
    static constexpr float KP_MIN       =  0.0f;
    static constexpr float KP_MAX       =  500.0f;
    static constexpr float KD_MIN       =  0.0f;
    static constexpr float KD_MAX       =  5.0f;

    // Refer to the Specification table of https://www.cubemars.com/product/ak10-9-v3-0-kv60-robotic-actuator.html
    static constexpr float RATED_TORQUE = 18.0f;

    // Bits 9-29 of the message ID
    static constexpr uint32_t FORCE_CONTROL_MODE = 0x800;
    static constexpr uint32_t REPLY_MESSAGE_CODE = 0x2900;
};

#endif