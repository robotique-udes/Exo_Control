/**
 * @file ICubemarsMotor.hpp
 * @brief Declaration of the ICubemarsMotor interface class
 * 
 * @author Samuel Savaria, Eloi Charbonneau, Gabriel Desrochers
 * @date 2026-06-05
 */
#ifndef BIOGENIUS_CUBEMARS_MOTOR_HPP
#define BIOGENIUS_CUBEMARS_MOTOR_HPP

#include <ESP32-TWAI-CAN.hpp>
#include <cstdint>
#include <limits>

#include "ICanObserver.hpp"

/**
 * @brief Error codes returned by Cubemars over CAN
*/
enum class CubemarsErrorCode : uint8_t
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
 * @brief Provides an interface for using a CubeMars motor in MIT mode.
 *
 * @details The constructor requires the 8-bits CAN ID of the motor
 *          The function sendCommand sends a MIT mode command to the motor
 *          The function notify takes a CAN message and parses it
 *          The parsed values can be obtained using their respective getter
 *          enterMode is used to enter MIT mode
 *
 * @author Samuel Savaria
 * @date 2026-06-05
 */
class ICubemarsMotor : public ICanObserver
{
public:
    /**
     * @brief Constructor
     *
     * @param[in] id The 8-bit CAN ID of the motor
     */
    ICubemarsMotor(uint8_t id);

    /**
     * @brief Enter MIT mode and reset the error state
     */
    virtual void enterMode() = 0;

    /**
     * @brief Sends a MIT mode command
     *
     * @param[in] position The target position in radians. Also requires KP and KD
     * @param[in] velocity The target velocity in radians/second. Also requires KD
     * @param[in] torque   The target torque in newtons-meter.
     * @param[in] kp       The proportional gain. Required by position
     * @param[in] kd       The derivative gain. Required by position and velocity
     */
    virtual void sendCommand(float position, float velocity, float torque, float kp, float kd) = 0;

    /**
     * @brief Parses a CAN message sent by the motor containing the position, speed, torque, temperature and error code
     *
     * @param[in] message The CAN message to parse
     */
    void notify(const CanFrame& message) override = 0;

    /**
     * @return The 8-bit CAN ID of the motor
     */
    uint8_t getMotorID() const;

    /**
     * @return The position of the motor, in radians
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
     * @return The temperature of the drive, in Celcius
     */
    int8_t getMosfetTemperature() const;

    /**
     * @brief Returns an error code. This value is reset by enterMode
     * 
     * @return The error code. See the error enum for each motor implementation
     * @retval CubemarsErrorCode::NO_FAULT: No errors
     */
    CubemarsErrorCode getErrorCode() const;

    /**
     * @brief Returns an error description corresponding to the error code
     * 
     * @return The error description
     */
    const char* getErrorDescription() const;

protected:
    /**
     * @brief CubeMars function to convert a float to an unsigned int, given a range and number of bits.
     *        All floats must be converted to unsigned ints before sending to the motor
     *
     * @param[in] value The value to convert
     * @param[in] min   The minimum value of the range
     * @param[in] max   The maximum value of the range
     * @param[in] bits  The number of bits in the resulting integer
     * 
     * @return The value converted to integer
     */
    static uint32_t float_to_uint(float value, float min, float max, uint8_t bits);

    /**
     * @brief CubeMars function to convert an unsigned int to a float, given a range and number of bits.
     *
     * @param[in] value The value to convert
     * @param[in] min   The minimum value of the range
     * @param[in] max   The maximum value of the range
     * @param[in] bits  The number of bits in the value to convert
     * 
     * @return The value converted to float
     */
    static float uint_to_float(uint32_t value, float min, float max, uint8_t bits);

    uint8_t m_motorId = 0;
    float m_position = std::numeric_limits<float>::quiet_NaN(); // Position in degrees
    float m_speed = std::numeric_limits<float>::quiet_NaN();    // Speed in RPM
    float m_torque = std::numeric_limits<float>::quiet_NaN();   // Torque in newton-meters
    int8_t m_temperature = INT8_MIN;                            // Temperature in Celcius
    CubemarsErrorCode m_errorCode = CubemarsErrorCode::NO_FAULT;

private:
    static constexpr uint8_t MAX_ERROR = 8;
    static constexpr const char* ERROR_DESCRIPTIONS[MAX_ERROR] = {
    "NO_FAULT",
    "MOTOR_OVER_TEMPERATURE",
    "OVER_CURRENT",
    "OVER_VOLTAGE",
    "UNDER_VOLTAGE",
    "ENCODER_FAULT",
    "MOSFET_OVER_TEMPERATURE",
    "MOTOR_STALL"
    };
};

#endif