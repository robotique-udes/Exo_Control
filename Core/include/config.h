#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace app::config
{
    namespace pins
    {
        constexpr uint8_t can_rx = 8;
        constexpr uint8_t can_tx = 9;
        constexpr uint8_t can_terminal = 4;
    }

    
    namespace motors
    {
        constexpr uint8_t amount = 4;
        constexpr uint8_t knee_left = 0;
        constexpr uint8_t knee_right = 1;
        constexpr uint8_t hip_left = 2;
        constexpr uint8_t hip_right = 3;

        constexpr uint8_t moving_avg_size = 10;
        constexpr float max_temperature = 60.0f;
        constexpr uint16_t shut_down_time = (10 * 1000);

        constexpr float p_max = 12.5f;
        constexpr float p_min = -12.5f;
        constexpr float v_max = 50.0f;
        constexpr float v_min = -50.0f;
        constexpr float kp_max = 500.0f;
        constexpr float kp_min = 0.0f;
        constexpr float kd_min = 0.0f;
        constexpr float kd_max = 5.0f;
        constexpr float torque_max = 1;
        constexpr float torque_multiplier = 0.1f;
        constexpr int force_control_mode = 0x800;
    }

    namespace anatomy 
    {
        constexpr float proportion_torso_length = 0.47f;
        constexpr float proportion_thigh_length = 0.245f;
        constexpr float proportion_calf_length = 0.285f;
    
        constexpr float proportion_torso_mass = 0.678f;
        constexpr float proportion_thigh_mass = 0.1f;
        constexpr float proportion_calf_mass = 0.061f;

        constexpr float max_hip_angle = 80;
        constexpr float min_hip_angle = -70;
        constexpr float max_knee_angle = 0;
        constexpr float min_knee_angle = -100;
        constexpr float angle_limit_tolerance = 10;
    }

    namespace physics
    {
        constexpr float exo_mass = 10;
        constexpr float gravity = 9.81;
    }

    namespace bnos
    {
        constexpr uint8_t buffer_size = 8;
        constexpr uint8_t amount = 6;
        constexpr uint8_t accel_threshold = 150;
        constexpr float accel_buffer_threshold = 0.5;

        constexpr uint8_t left_thigh = 0;
        constexpr uint8_t right_thigh = 1;
        constexpr uint8_t left_shin = 2;
        constexpr uint8_t right_shin = 3;
        constexpr uint8_t exo_back = 4;
        constexpr uint8_t mobo = 5;

        constexpr uint8_t nb_leg = 2;
        constexpr uint8_t left_leg = 0;
        constexpr uint8_t right_leg = 1;
    }

    namespace path
    {
        constexpr uint8_t left_moustache_channel = 0;
        constexpr uint8_t back_channel = 1;
        constexpr uint8_t right_moustache_channel = 2;
        constexpr uint8_t mobo_channel = 3;
        constexpr int address_1 = 0x4A;
        constexpr int address_2 = 0x4B;
    }

    namespace debug
    {
        constexpr bool bno = true;
        constexpr bool logic = true;
        constexpr bool motor_handler = true;
        constexpr bool motor = true;
        constexpr bool main = true;
    }
}  
#endif