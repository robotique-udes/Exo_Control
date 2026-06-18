/**
 * @file MotorHandler.hpp
 * @brief Declaration of the MotorHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-06-10
 */
#ifndef BIOGENIUS_MOTOR_HANDLER_HPP
#define BIOGENIUS_MOTOR_HANDLER_HPP

#include <array>

#include "config.h"
#include "CubemarsMotorV2.hpp"
#include "CubemarsMotorV3.hpp"
#include "MovingAverage.hpp"

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
     */
    MotorHandler();

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
    void update(std::array<float, app::config::motors::amount> torques);

private:
    struct Motor
    {
        ICubemarsMotor* motor;
        MovingAverage avg;
    };

    std::array<Motor, app::config::motors::amount> m_motors;
    CubemarsMotorV2 m_kneeLeft = CubemarsMotorV2(app::config::motors::knee_left);
    CubemarsMotorV2 m_kneeRight = CubemarsMotorV2(app::config::motors::knee_right);
    CubemarsMotorV3 m_hipLeft = CubemarsMotorV3(app::config::motors::hip_left);
    CubemarsMotorV3 m_hipRight = CubemarsMotorV3(app::config::motors::hip_right);

    bool m_enabled = false;
};

#endif