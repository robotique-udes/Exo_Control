/**
 * @file MotorHandler.hpp
 * @brief Declaration of the MotorHandler class
 * 
 * @author Samuel Savaria, Gabriel Desrochers
 * @date 2026-06-10
 */

#ifndef BIOGENIUS_MOTOR_HANDLER_HPP
#define BIOGENIUS_MOTOR_HANDLER_HPP

#include <array>

#include "config.hpp"
#include "ICubemarsMotor.hpp"
#include "ICubemarsMotor.hpp"
#include "utils/MovingAverage.hpp"

/**
 * @brief Owns all motors and passes the torques through safety mechanisms before sending them
 *
 * @details Passes the torques through the following safety mechanisms:
 *          1) Disables the motors if they are too hot or in a error state
 *          2) Limits the maximum torque the motors can receive to avoid excessive torques
 *          3) Sets the torques to 0 if the motors are disabled to stop the motors
 *          4) Pass the torques through a moving average to avoid sudden changes
 *
 * @author Samuel Savaria
 * @date 2026-06-10
 */
class MotorHandler
{
public:
    /**
     * @brief Constructor
     *
     * @param[in] p_motors Array of motors, indexed by their exo_config::motors index
     */
    MotorHandler(std::array<ICubemarsMotor*, exo_config::motors::AMOUNT> p_motors);

    /**
     * @brief Enables all motors
     */
    void enableMotors();

    /**
     * @brief Disables all motors
     */
    void disableMotors();

    /**
     * @brief Sends new torques to the motors
     *
     * @param[in] torques Array of torques. The index of the array corresponds to the CAN ID of the motor
     */
    void update(std::array<float, exo_config::motors::AMOUNT> torques);

private:
    struct Motor
    {
        ICubemarsMotor* motor;
        MovingAverage avg;
    };

    std::array<Motor, exo_config::motors::AMOUNT> m_motors;

    bool m_enabled = false;
};

#endif