#include "motorHandler.h"

MotorHandler::MotorHandler()
{
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
    //for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    //    motors[motorPos].enterMode();
}

void MotorHandler::exitMotors()
{
    //for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    //    motors[motorPos].exitMode();
}

void MotorHandler::applyTorque()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {
        EnumMotorPosition motorID = static_cast<EnumMotorPosition> (motorPos);
        float torque = dataCore.getPWM(motorID);
        motors[motorPos].applyTorque(TORQUE, torque);
        float temperature = motors[motorPos].getTemperature();
        if (temperature > TEMP_THRESHOLD)
        {
            tempTooHigh = true;
            return;
        }
    }
}

//TODO tester le slow shutdown
void MotorHandler::slowShutDown()
{
    unsigned long currentTime = millis();
    unsigned long deltaTime = currentTime - previousTime;
    totalTime += deltaTime;

    //if the timer is done, shuts down all the motors and waits for reactivation
    if (totalTime >= SHUT_DOWN_TIME)
    {  
        totalTime = 0;
        tempTooHigh = false;
        exitMotors();
        return;
    }

    //if not, slowly reduces the torque until the timer ends
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
    {  
        float torqueLoss = (initialTorque[motorPos] / SHUT_DOWN_TIME) * deltaTime;
        float torque = motors[motorPos].getCurrentTorque() - torqueLoss;
        motors[motorPos].applyTorque(TORQUE, torque);
    }
    previousTime = currentTime;
}

