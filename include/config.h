#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace exo_config
{
    namespace pins
    {
        constexpr uint8_t CAN_RX = 8;
        constexpr uint8_t CAN_TX = 9;
        constexpr uint8_t CAN_TERMINAL = 4;
    }

    namespace motors
    {
        constexpr uint8_t AMOUNT = 4;
        constexpr uint8_t KNEE_LEFT = 0;
        constexpr uint8_t KNEE_RIGHT = 1;
        constexpr uint8_t HIP_LEFT = 2;
        constexpr uint8_t HIP_RIGHT = 3;

        constexpr unsigned long MOVING_AVG_LENGTH = 1000; // In milliseconds
        constexpr float MAX_TEMPERATURE = 60.0f; // In Celcius
        constexpr float TORQUE_MAX = 8; // In newtons-meter

        constexpr float TORQUE_MULTIPLIER = 0.2f;
    }

    namespace anatomy 
    {
        constexpr float PROPORTION_TORSO_LENGTH = 0.47f;
        constexpr float PROPORTION_THIGH_LENGTH = 0.245f;
        constexpr float PROPORTION_CALF_LENGTH = 0.285f;
    
        constexpr float PROPORTION_TORSO_MASS = 0.678f;
        constexpr float PROPORTION_THIGH_MASS = 0.1f;
        constexpr float PROPORTION_CALF_MASS = 0.061f;

        constexpr float MAX_HIP_ANGLE = 80;
        constexpr float MIN_HIP_ANGLE = -70;
        constexpr float MAX_KNEE_ANGLE = 0;
        constexpr float MIN_KNEE_ANGLE = -100;
        constexpr float ANGLE_LIMIT_TOLERANCE = 10;
    }

    namespace physics
    {
        constexpr float EXO_MASS = 10;
        constexpr float GRAVITY = 9.81;
    }

    namespace bnos
    {
        constexpr uint8_t BUFFER_SIZE = 8;
        constexpr uint8_t AMOUNT = 6;
        constexpr uint8_t ACCEL_THRESHOLD = 150;
        constexpr float ACCEL_BUFFER_THRESHOLD = 0.5;

        constexpr uint8_t LEFT_THIGH = 0;
        constexpr uint8_t RIGHT_THIGH = 1;
        constexpr uint8_t LEFT_SHIN = 2;
        constexpr uint8_t RIGHT_SHIN = 3;
        constexpr uint8_t EXO_BACK = 4;
        constexpr uint8_t MOBO = 5;

        constexpr uint8_t NB_LEG = 2;
        constexpr uint8_t LEFT_LEG = 0;
        constexpr uint8_t RIGHT_LEG = 1;
    }

    namespace path
    {
        constexpr uint8_t LEFT_MOUSTACHE_CHANNEL = 0;
        constexpr uint8_t BACK_CHANNEL = 1;
        constexpr uint8_t RIGHT_MOUSTACHE_CHANNEL = 2;
        constexpr uint8_t MOBO_CHANNEL = 3;
        constexpr int ADDRESS_1 = 0x4A;
        constexpr int ADDRESS_2 = 0x4B;
    }

    namespace debug
    {
        constexpr bool BNO = true;
        constexpr bool LOGIC = true;
        constexpr bool MOTOR_HANDLER = true;
        constexpr bool MOTOR = true;
        constexpr bool MAIN = true;
    }
}  
#endif