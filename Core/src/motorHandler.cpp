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
    //iterate for all motors 
    for (int motorIndex = 0; motorIndex < NB_MOTORS; motorIndex++)
    {
        //send torque request to the respective motor
        float torque = dataCore.getPWM(static_cast<EnumMotorPosition>(motorIndex));
        motors[motorIndex].sendRequest(TORQUE, torque);
        initialTorque[motorIndex] = torque;
        //checks if the respective motor is overheating
        float temperature = motors[motorIndex].getTemperature();
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
    for (int motorIndex = 0; motorIndex < NB_MOTORS; motorIndex++)
    {  
        float torqueLoss = (initialTorque[motorIndex] / SHUT_DOWN_TIME) * deltaTime;
        float torque = motors[motorIndex].getCurrentTorque() - torqueLoss;
        motors[motorIndex].sendRequest(TORQUE, torque);
    }
    previousTime = currentTime;
}


