/**
 * @file CubemarsMotorV2.hpp
 * @brief Declaration of the CubemarsMotorV2 implementation class
 * @see Datasheet: https://www.cubemars.com/data/cms/202604/ak-series-driver-manual-v1-0-18-for-ak-2-0-robotic-actuator.pdf
 * 
 * @author Samuel Savaria
 * @date 2026-06-07
 */
#ifndef BIOGENIUS_CUBEMARS_MOTOR_V2_HPP
#define BIOGENIUS_CUBEMARS_MOTOR_V2_HPP

#include "ICubemarsMotor.hpp"

/**
 * @brief Implementation of the ICubemarsMotor interface for a CubeMars V2 motor
 *
 * @warning The parameter range for the AK10-9 KV100 V2 model is hardcoded in this class
 *          If more V2 models were to be used, the parameters would need to be member variables instead of class attributes
 *
 * @author Samuel Savaria
 * @date 2026-06-07
 */
class CubemarsMotorV2 : public ICubemarsMotor
{
public:
/**
     * @brief Constructor
     *
     * @param[in] id The 8-bit CAN ID of the motor
     */
    CubemarsMotorV2(uint8_t id);

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
    /**
     * @brief Enter MIT mode. Cubemars V2 motors have two seperate modes for Servo and MIT.
     */
    void enterMitMode();

    /**
     * @brief Set the current position as 0
     */
    void zeroSet();

    // Refer to section 5.3 of the datasheet for the parameter range of the AK10-9
    static constexpr float POSITION_MIN = -12.5f;
    static constexpr float POSITION_MAX =  12.5f;
    static constexpr float VELOCITY_MIN = -50.0f;
    static constexpr float VELOCITY_MAX =  50.0f;
    static constexpr float TORQUE_MIN   = -65.0f;
    static constexpr float TORQUE_MAX   =  65.0f;
    static constexpr float KP_MIN       =  0.0f;
    static constexpr float KP_MAX       =  500.0f;
    static constexpr float KD_MIN       =  0.0f;
    static constexpr float KD_MAX       =  5.0f;

    // Refer to the Specification table of https://www.cubemars.com/product/ak10-9-v2-0-kv100-robotic-actuator.html
    static constexpr float RATED_TORQUE = 18.0f;
};

#endif