/**
 * @file mitModeMotor.h
 * @brief Declaration of the IMitModeMotor interface class
 * 
 * @author Samuel Savaria
 * @date 2026-06-05
 */
#ifndef BIOGENIUS_MIT_MODE_MOTOR_H
#define BIOGENIUS_MIT_MODE_MOTOR_H

#include <ESP32-TWAI-CAN.hpp>
#include <cstdint>
#include <limits>

enum class CubeMarsErrorCode : uint8_t
{
    NoFault = 0,
    MotorOverTemperature = 1,
    OverCurrent = 2,
    OverVoltage = 3,
    UnderVoltage = 4,
    EncoderFault = 5,
    MosfetOverTemperature = 6,
    MotorStall = 7
};

/**
 * @brief Provides an interface for using a CubeMars motor in MIT mode.
 *
 * @details The constructor requires the 8-bits CAN ID of the motor
 *          The function sendCommand sends a MIT mode command to the motor
 *          The function receiveCommand takes a CAN message and parses it
 *          The parsed values can be obtained using their respective getter
 *          enterMode and exitMode are used to enter and exit MIT mode
 *
 * @author Samuel Savaria
 * @date 2026-06-05
 */
class IMitModeMotor
{
protected:
    uint8_t motorID = 0;
    float position = std::numeric_limits<float>::quiet_NaN(); // Position in degrees
    float speed = std::numeric_limits<float>::quiet_NaN();    // Speed in RPM
    float torque = std::numeric_limits<float>::quiet_NaN();   // Torque in newton-meters
    int8_t temperature = INT8_MIN;                            // Temperature in Celcius
    CubeMarsErrorCode errorCode = CubeMarsErrorCode::NoFault;

    /**
     * @brief CubeMars function to convert a float to an unsigned int, given a range and number of bits.
     *        All floats must be converted to unsigned ints before sending to the motor
     *
     * @param value The value to convert
     * @param min   The minimum value of the range
     * @param max   The maximum value of the range
     * @param bits  The number of bits in the resulting integer
     * 
     * @return The value converted to integer
     */
    static uint32_t float_to_uint(float value, float min, float max, uint8_t bits);

    /**
     * @brief CubeMars function to convert an unsigned int to a float, given a range and number of bits.
     *
     * @param value The value to convert
     * @param min   The minimum value of the range
     * @param max   The maximum value of the range
     * @param bits  The number of bits in the value to convert
     * 
     * @return The value converted to float
     */
    static float uint_to_float(uint32_t value, float min, float max, uint8_t bits);

public:
    /**
     * @brief Constructor
     *
     * @param id The 8-bit CAN ID of the motor
     */
    IMitModeMotor(uint8_t id);

    /**
     * @brief Enter MIT mode
     */
    virtual void enterMode() = 0;

    /**
     * @brief Sends a MIT mode command
     *
     * @param position The target position in radians. Also requires KP and KD
     * @param velocity The target velocity in radians/second. Also requires KD
     * @param torque   The target torque in newtons-meter.
     * @param kp       The proportional gain. Required by position
     * @param kd       The derivative gain. Required by position and velocity
     */
    virtual void sendCommand(float position, float velocity, float torque, float kp, float kd) = 0;
    
    /**
     * @brief Parses a CAN message sent by the motor containing the position, speed, torque, temperature and error code
     *
     * @param message The CAN message to parse
     */
    virtual void receiveCommand(const CanFrame& message) = 0;

    /**
     * @return The 8-bit CAN ID of the motor
     */
    uint8_t getMotorID() const;

    /**
     * @return The position of the motor, in degrees
     */
    float getPosition() const;

    /**
     * @return The speed of the motor, in RPM
     */
    float getSpeed() const;

    /**
     * @return The torque consumed by the motor, in newton-meters
     */
    float getTorque() const;

    /**
     * @return The temperature of the motor, in Celcius
     */
    int8_t getTemperature() const;

    /**
     * @return The motor error code. The value is reset by enterMode.
     */
    CubeMarsErrorCode getErrorCode() const;
};

#endif