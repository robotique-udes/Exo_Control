#include "motorHandler.h"

MotorHandler::MotorHandler()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {
        motors[motorPos].setMotorPosition(static_cast<EnumMotorPosition> (motorPos));
    }
    initializeMotors();
}


MotorHandler::~MotorHandler()
{
    exitMotors();
}

void MotorHandler::Update()
{
    if (tempTooHigh == true)
    {
        slowShutDown();
        return;
    }

    applyTorque();
}

void MotorHandler::initializeMotors()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
        motors[motorPos].enterMode();
}

void MotorHandler::exitMotors()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
        motors[motorPos].exitMode();
}

void MotorHandler::applyTorque()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {
        EnumMotorPosition motorID = static_cast<EnumMotorPosition> (motorPos);
        currentTorque[motorPos] = dataCore.getPWM(motorID);

        motors[motorPos].sendCommand(TORQUE, currentTorque[motorPos]);

        float temperature = motors[motorPos].getTemperature();

        if (temperature > TEMP_THRESHOLD)
        {
            tempTooHigh = true;
            return;
        }
    }
}

void MotorHandler::slowShutDown()
{
    unsigned long currentTime = millis();

    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {
        int multiplier = 1;
        if (currentTorque[motorPos] < 0)
            multiplier = -1;

        currentTorque[motorPos] -= multiplier * SHUT_DOWN_SPEED * (currentTime - previousTime);

        if (multiplier == 1 && currentTorque[motorPos] <= 0)
            currentTorque[motorPos] = 0;
        else if (multiplier == -1 && currentTorque[motorPos] >= 0)
            currentTorque[motorPos] = 0;

        motors[motorPos].sendCommand(TORQUE, currentTorque[motorPos]);
    }
    tempTooHigh = false;
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {
        if (currentTorque[motorPos] != 0)
            tempTooHigh = true;
    }

    if (tempTooHigh == false)
        exitMotors();
}

