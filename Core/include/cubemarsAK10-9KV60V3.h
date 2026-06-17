/**
 * @file CubemarsAK10-9KV60V3.h
 * @brief Declaration of the Cubemars_AK10_9_KV60_V3 implementation class
 * @see Datasheet: https://www.cubemars.com/images/file/20241217/1734428582786471.pdf
 * 
 * @author Samuel Savaria
 * @date 2026-06-06
 */
#ifndef BIOGENIUS_CUBEMARS_AK10_9_KV60_V3_H
#define BIOGENIUS_CUBEMARS_AK10_9_KV60_V3_H

#include "MitModeMotor.h"

enum CubemarsV3ErrorCode : uint8_t
{
    NO_FAULT = 0,
    MOTOR_OVER_TEMPERATURE = 1,
    OVER_CURRENT = 2,
    OVER_VOLTAGE = 3,
    UNDER_VOLTAGE = 4,
    ENCODER_FAULT = 5,
    MOSFET_OVER_TEMPERATURE = 6,
    MOTOR_STALL = 7
};

/**
 * @brief Implementation of the IMitModeMotor interface for a CubeMars AK10-9 KV60 V3 motor
 *
 * @details The class contains the parameter range and errors for the AK10-9 KV60 V3 model
 *          and implements the interface's pure virtual functions
 *
 * @author Samuel Savaria
 * @date 2026-06-06
 */
class Cubemars_AK10_9_KV60_V3 : public IMitModeMotor
{
public:
/**
     * @brief Constructor
     *
     * @param[in] id The 8-bit CAN ID of the motor
     */
    Cubemars_AK10_9_KV60_V3(uint8_t id);

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

    /**
     * @brief Returns an error description corresponding to the error code
     * 
     * @return The error description
     */
    const char* getErrorDescription() const override;

private:
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
    static constexpr uint32_t FORCE_CONTROL_MODE = 0x800;
    static constexpr uint32_t REPLY_MESSAGE_CODE = 0x2900;
    static constexpr uint8_t ERROR_COUNT = 8; // The number of possible errors
    static constexpr const char* ERROR_DESCRIPTIONS[ERROR_COUNT] = {
    "NoFault",
    "MotorOverTemperature",
    "OverCurrent",
    "OverVoltage",
    "UnderVoltage",
    "EncoderFault",
    "MosfetOverTemperature",
    "MotorStall"
    };
};

#endif