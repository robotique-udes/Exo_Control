/**
 * @file MotorHandler.cpp
 * @brief Implementation of the MotorHandler class
 * 
 * @author Samuel Savaria, Gabriel Desrochers
 * @date 2026-06-10
 */
#include "MotorHandler.hpp"

#define DEBUG_MOTOR_HANDLER // uncomment to display prints in this file
#ifdef DEBUG_MOTOR_HANDLER
#define PRINT(x) Serial.print(x)
#define PRINTLN(x) Serial.println(x)
#define PRINTF(...) Serial.printf(__VA_ARGS__)
#else 
#define PRINT(x)
#define PRINTLN(x)
#define PRINTF(...)
#endif


MotorHandler::MotorHandler()
{
    m_motors[exo_config::motors::KNEE_LEFT].motor = &m_kneeLeft;
    m_motors[exo_config::motors::KNEE_RIGHT].motor = &m_kneeRight;
    m_motors[exo_config::motors::HIP_LEFT].motor = &m_hipLeft;
    m_motors[exo_config::motors::HIP_RIGHT].motor = &m_kneeRight;

    for(Motor motor : m_motors)
    {
        motor.avg = MovingAverage(exo_config::motors::MOVING_AVG_LENGTH);
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

void MotorHandler::update(std::array<float, exo_config::motors::AMOUNT> p_torques)
{
    CanFrame msg;
    while(ESP32Can.readFrame(&msg, 0))
    {
        for(Motor& motor : m_motors)
        {
            motor.motor->receiveCommand(msg);
        }
    }

    for(uint8_t i = 0; i < exo_config::motors::AMOUNT; ++i)
    {
        // 1) Disables the motors if they are too hot to prevent overheating
        if(m_motors[i].motor->getMosfetTemperature() > exo_config::motors::MAX_TEMPERATURE ||
           m_motors[i].motor->getErrorCode() != CubemarsErrorCode::NO_FAULT)
        {
            disableMotors();
        }

        // 2) Limits the maximum torque the motors can receive to avoid excessive torques
        p_torques[i] = constrain(p_torques[i], -exo_config::motors::TORQUE_MAX, exo_config::motors::TORQUE_MAX);

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