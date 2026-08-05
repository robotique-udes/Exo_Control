 /** 
 * @file Config.hpp
 * @brief Configuration file for this exosqueleton
 * @author Gabriel Desrochers
 * @date 2026-07-18
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>


/**
 * @brief Global configurations for the exosqueleton parameters.
 */
namespace exo_config
{
    /**
     * @brief PIN configuration used by the hardware interfaces.
     */
    namespace pins
    {
        constexpr uint8_t CAN_RX = 8;       ///< CAN receive pin.
        constexpr uint8_t CAN_TX = 9;       ///< CAN transmit pin.
        constexpr uint8_t CAN_TERMINAL = 4; ///< CAN terminal enable pin.
    }

    /**
     * @brief MotorHandler-related constants used for torque control and indexing.
     */
    namespace motors
    {
        constexpr uint8_t AMOUNT = 4;       ///< Number of controlled motors.
        constexpr uint8_t KNEE_LEFT = 0;    ///< Index of the left knee motor.
        constexpr uint8_t KNEE_RIGHT = 1;   ///< Index of the right knee motor.
        constexpr uint8_t HIP_LEFT = 2;     ///< Index of the left hip motor.
        constexpr uint8_t HIP_RIGHT = 3;    ///< Index of the right hip motor.

        constexpr unsigned long MOVING_AVG_LENGTH = 1000;   ///< Moving-average window length in milliseconds.
        constexpr float MAX_TEMPERATURE = 60.0f;            ///< Maximum allowed motor temperature in degrees Celsius.
        constexpr float TORQUE_MAX = 8;                     ///< Maximum allowed torque in newton-meters.

        constexpr float TORQUE_MULTIPLIER = 0.2f;   ///< Scaling factor applied to torque commands.
    }

    /**
     * @brief Biomechanical proportions and joint-angle limits of the user model.
     */
    namespace anatomy 
    {
        constexpr float PROPORTION_TORSO_LENGTH = 0.47f;    ///< Torso length proportion of the total body height.
        constexpr float PROPORTION_THIGH_LENGTH = 0.245f;   ///< Thigh length proportion of the total body height.
        constexpr float PROPORTION_CALF_LENGTH = 0.285f;    ///< Calf length proportion of the total body height.
    
        constexpr float PROPORTION_TORSO_MASS = 0.678f; ///< Torso mass proportion of the total body mass.
        constexpr float PROPORTION_THIGH_MASS = 0.1f;   ///< Thigh mass proportion of the total body mass.
        constexpr float PROPORTION_CALF_MASS = 0.061f;  ///< Calf mass proportion of the total body mass.

        constexpr float MAX_HIP_ANGLE = 80;     ///< Maximum allowed hip angle in degrees.
        constexpr float MIN_HIP_ANGLE = -70;    ///< Minimum allowed hip angle in degrees.
        constexpr float MAX_KNEE_ANGLE = 0;     ///< Maximum allowed knee angle in degrees.
        constexpr float MIN_KNEE_ANGLE = -100;  ///< Minimum allowed knee angle in degrees.
        constexpr float ANGLE_LIMIT_TOLERANCE = 10; ///< Safety tolerance around joint-angle limits.
    }

    /**
     * @brief Physical constants used by the control model.
     */
    namespace physics
    {
        constexpr float EXO_MASS = 10;  ///< Estimated mass of the exoskeleton in kilograms.
        constexpr float GRAVITY = 9.81; ///< Standard gravity constant in m/s².
    }

    /**
     * @brief BNO sensor indexing and thresholds used for posture estimation.
     */
    namespace bnos
    {
        constexpr uint8_t BUFFER_SIZE = 8;  ///< Number of samples stored in the acceleration buffer.
        constexpr uint8_t AMOUNT = 6;       ///< Number of BNO sensors managed by the system.
        constexpr uint8_t ACCEL_THRESHOLD = 150;        ///< Threshold for detecting foot contact from acceleration.
        constexpr float ACCEL_BUFFER_THRESHOLD = 0.5;   ///< Minimum average buffer value to consider the leg grounded.

        constexpr uint8_t LEFT_THIGH = 0;   ///< Index of the left thigh sensor.
        constexpr uint8_t RIGHT_THIGH = 1;  ///< Index of the right thigh sensor.
        constexpr uint8_t LEFT_SHIN = 2;    ///< Index of the left shin sensor.
        constexpr uint8_t RIGHT_SHIN = 3;   ///< Index of the right shin sensor.
        constexpr uint8_t EXO_BACK = 4;     ///< Index of the back sensor.
        constexpr uint8_t MOBO = 5;         ///< Index of the motherboard sensor.

        constexpr uint8_t NB_LEG = 2;   ///< Number of legs handled by the control logic.
        constexpr uint8_t LEFT_LEG = 0; ///< Index of the left leg in the grounded-state array.
        constexpr uint8_t RIGHT_LEG = 1;///< Index of the right leg in the grounded-state array.
    }

    /**
     * @brief I2C path and address constants used for the sensor multiplexer.
     */
    namespace path
    {
        constexpr uint8_t LEFT_MOUSTACHE_CHANNEL = 0;   ///< Multiplexer channel for the left moustache.
        constexpr uint8_t BACK_CHANNEL = 1;             ///< Multiplexer channel for the back.
        constexpr uint8_t RIGHT_MOUSTACHE_CHANNEL = 2;  ///< Multiplexer channel for the right moustache.
        constexpr uint8_t MOBO_CHANNEL = 3;             ///< Multiplexer channel for the motherboard.
        constexpr int ADDRESS_1 = 0x4A;                 ///< First I2C address used by thes.
        constexpr int ADDRESS_2 = 0x4B;                 ///< Second I2C address used by thes.
    }

    /**
     * @brief Debug flags enabling or disabling runtime logs for specific components.
     */
    namespace debug
    {
        constexpr bool BNO = true;          ///< Enable BNO-related debug output.
        constexpr bool LOGIC = true;        ///< Enable logic-related debug output.
        constexpr bool MOTOR_HANDLER = true;///< Enable motor handler debug output.
        constexpr bool MOTOR = true;        ///< Enable motor debug output.
        constexpr bool MAIN = true;         ///< Enable main application debug output.
    }
}  
#endif