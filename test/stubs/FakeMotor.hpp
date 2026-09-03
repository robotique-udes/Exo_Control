/**
 * @file FakeMotor.hpp
 * @brief Native test double for ICubemarsMotor, for injecting into modules that take motors via their constructor
 */
#ifndef TEST_STUBS_FAKE_MOTOR_HPP
#define TEST_STUBS_FAKE_MOTOR_HPP

#include "ICubemarsMotor.hpp"

class FakeMotor : public ICubemarsMotor
{
public:
    FakeMotor(uint8_t id) : ICubemarsMotor(id) {}

    void enterMode() override
    {
        ++enterModeCalls;
        m_errorCode = CubemarsErrorCode::NO_FAULT;
    }

    void sendCommand(float, float, float torque, float, float) override
    {
        ++sendCommandCalls;
        lastTorque = torque;
    }

    void notify(const CanFrame&) override {}

    void setTemperature(int8_t temperature) { m_temperature = temperature; }
    void setErrorCode(CubemarsErrorCode error) { m_errorCode = error; }

    int enterModeCalls = 0;
    int sendCommandCalls = 0;
    float lastTorque = 0.0f;
};

#endif
