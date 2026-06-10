/**
 * @file motorHandler.cpp
 * @brief Implementation of the MotorHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-06-10
 */
#include "motorHandler.h"

MotorHandler::MotorHandler()
{
    motors[app::config::motors::knee_left].motor = &kneeLeft;
    motors[app::config::motors::knee_right].motor = &kneeRight;
    motors[app::config::motors::hip_left].motor = &hipLeft;
    motors[app::config::motors::hip_right].motor = &kneeRight;

    for(Motor motor : motors)
    {
        motor.avg = MovingAverage(app::config::motors::moving_avg_length);
    }

    enabled = false;
}

void MotorHandler::enableMotors()
{
    // Enter MIT mode and reset the moving average
    for(Motor motor : motors)
    {
        motor.motor->enterMode();
        motor.avg.reset();
    }

    enabled = true;
}

void MotorHandler::disableMotors()
{
    enabled  = false;
}

void MotorHandler::update(std::array<float, app::config::motors::amount> torques)
{
    CanFrame msg;
    while(ESP32Can.readFrame(&msg, 0))
    {
        for(Motor motor : motors)
        {
            motor.motor->receiveCommand(msg);
        }
    }

    for(uint8_t i = 0; i < app::config::motors::amount; ++i)
    {
        // 1) Disables the motors if they are too hot to prevent overheating
        if(motors[i].motor->getTemperature() > app::config::motors::max_temperature ||
           motors[i].motor->getErrorCode() != CubeMarsErrorCode::NoFault)
        {
            disableMotors();
        }

        // 2) Limits the maximum torque the motors can receive to avoid excessive torques
        torques[i] = constrain(torques[i], -app::config::motors::torque_max, app::config::motors::torque_max);

        // 3) Sets the torques to 0 if the motors are disabled to stop the motors
        if(!enabled)
        {
            torques[i] = 0.0f;
        }

        // 4) Pass the torques through a moving average to avoid sudden changes
        torques[i] = motors[i].avg.addValue(torques[i]);

        motors[i].motor->sendCommand(0.0f, 0.0f, torques[i], 0.0f, 0.0f);
    }
}