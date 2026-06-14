/**
 * @file motorHandler.h
 * @brief Declaration of the MotorHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-06-10
 */
#ifndef BIOGENIUS_MOTOR_HANDLER_H
#define BIOGENIUS_MOTOR_HANDLER_H

#include <array>

#include "config.h"
#include "cubemarsAK10-9KV100V2.h"
#include "cubemarsAK10-9KV60V3.h"
#include "movingAverage.h"

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
private:
    struct Motor
    {
        IMitModeMotor* motor;
        MovingAverage avg;
    };

    std::array<Motor, app::config::motors::amount> motors;
    Cubemars_AK10_9_KV100_V2 kneeLeft = Cubemars_AK10_9_KV100_V2(app::config::motors::knee_left);
    Cubemars_AK10_9_KV100_V2 kneeRight = Cubemars_AK10_9_KV100_V2(app::config::motors::knee_right);
    Cubemars_AK10_9_KV60_V3 hipLeft = Cubemars_AK10_9_KV60_V3(app::config::motors::hip_left);
    Cubemars_AK10_9_KV60_V3 hipRight = Cubemars_AK10_9_KV60_V3(app::config::motors::hip_right);

    bool enabled = false;

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
     * @param torques Array of torques. The index of the array corresponds to the CAN ID of the motor
     */
    void update(std::array<float, app::config::motors::amount> torques);
};

#endif