/**
 * @file MotorHandler.cpp
 * @brief Implementation of the MotorHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-06-10
 */
#include "MotorHandler.hpp"

MotorHandler::MotorHandler()
{
    m_motors[app::config::motors::knee_left].motor = &m_kneeLeft;
    m_motors[app::config::motors::knee_right].motor = &m_kneeRight;
    m_motors[app::config::motors::hip_left].motor = &m_hipLeft;
    m_motors[app::config::motors::hip_right].motor = &m_kneeRight;

    for(Motor motor : m_motors)
    {
        motor.avg = MovingAverage(app::config::motors::moving_avg_length);
    }

    m_enabled = false;
}

void MotorHandler::enableMotors()
{
    for(Motor& motor : m_motors)
    {
        motor.motor->enterMode();
        motor.avg.reset();
    }

    m_enabled = true;
}

void MotorHandler::disableMotors()
{
    m_enabled  = false;
}

void MotorHandler::update(std::array<float, app::config::motors::amount> p_torques)
{
    CanFrame msg;
    while(ESP32Can.readFrame(&msg, 0))
    {
        for(Motor& motor : m_motors)
        {
            motor.motor->receiveCommand(msg);
        }
    }

    for(uint8_t i = 0; i < app::config::motors::amount; ++i)
    {
        // 1) Disables the motors if they are too hot to prevent overheating
        if(m_motors[i].motor->getMosfetTemperature() > app::config::motors::max_temperature ||
           m_motors[i].motor->getErrorCode() != 0)
        {
            disableMotors();
        }

        // 2) Limits the maximum torque the motors can receive to avoid excessive torques
        p_torques[i] = constrain(p_torques[i], -app::config::motors::torque_max, app::config::motors::torque_max);

        // 3) Sets the torques to 0 if the motors are disabled to stop the motors
        if(!m_enabled)
        {
            p_torques[i] = 0.0f;
        }

        // 4) Pass the torques through a moving average to avoid sudden changes
        p_torques[i] = m_motors[i].avg.addValue(p_torques[i]);

        m_motors[i].motor->sendCommand(0.0f, 0.0f, p_torques[i], 0.0f, 0.0f);
    }
}