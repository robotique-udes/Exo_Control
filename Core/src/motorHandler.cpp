#include "motorHandler.h"

MotorHandler::MotorHandler()
{
    stateMutex = xSemaphoreCreateMutex();

    motors[0] = new MotorV3(static_cast<int>(EnumMotorPosition::HIP_R));
    motors[1] = new MotorV2(static_cast<int>(EnumMotorPosition::HIP_L));
    motors[2] = new MotorV3(static_cast<int>(EnumMotorPosition::KNEE_R)); 
    motors[3] = new MotorV2(static_cast<int>(EnumMotorPosition::KNEE_L));
}

MotorHandler::~MotorHandler()
{
    exitMotors();

    delete motors[0];
    delete motors[1];
    delete motors[2];
    delete motors[3];
}

void MotorHandler::Update(const float torque[NB_MOTORS])
{
    if (tempTooHigh == true)
    {
        //slowShutDown();
        //TODO remplacer ca par le slow shutdown
        float shutDown[NB_MOTORS] = {0,0,0,0};
        applyTorque(shutDown);
        return;
    }

    applyTorque(torque);
}

void MotorHandler::initializeMotors()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
        motors[motorPos]->start();

    initialized = true;
}

void MotorHandler::exitMotors()
{
    for (int motorPos = 0; motorPos < NB_MOTORS; motorPos++)
        motors[motorPos]->stop();

    initialized = false;
}

void MotorHandler::applyTorque(const float torque[NB_MOTORS])
{
    for (int motorIndex = 0; motorIndex < NB_MOTORS; motorIndex++)
    {
        float motorTorque = 0.0f;
        if(initialized)
        {
            for(int sampleIndex = 0; sampleIndex < (SAMPLE_COUNT - 1); ++sampleIndex)
            {
                movingAverage[motorIndex][sampleIndex] = movingAverage[motorIndex][sampleIndex + 1];
                motorTorque += movingAverage[motorIndex][sampleIndex];
            }

            movingAverage[motorIndex][SAMPLE_COUNT - 1] = torque[motorIndex];
            motorTorque += movingAverage[motorIndex][SAMPLE_COUNT - 1];

            motorTorque /= SAMPLE_COUNT;
        }

        
        if (motorIndex == static_cast<int>(EnumMotorPosition::HIP_L)
            || motorIndex == static_cast<int> (EnumMotorPosition::KNEE_L))
        {
            motorTorque = -1*motorTorque;
        }  

        //send torque request to the respective motor
        if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) 
        {
            motors[motorIndex]->sendRequest(TORQUE, motorTorque*motorOn);
            xSemaphoreGive(stateMutex);
        }
        initialShutdownTorque[motorIndex] = motorTorque;

        //checks if the respective motor is overheating
        float temperature = motors[motorIndex]->getTemperature();
        if (temperature > TEMP_THRESHOLD)
        {
            tempTooHigh = true;
            shutdownStartTime = millis();
            return;
        }
    }
}

//TODO tester le slow shutdown
void MotorHandler::slowShutDown()
{
    unsigned long timeElapsed = millis() - shutdownStartTime;

    //if the timer is done, shuts down all the motors
    if (timeElapsed > SHUT_DOWN_TIME * 1000)
    {  
        tempTooHigh = false;
        exitMotors();
        return;
    }

    //if not, linearly reduces the torque until the timer ends
    for (int motorIndex = 0; motorIndex < NB_MOTORS; motorIndex++)
    {  
        float torque = initialShutdownTorque[motorIndex] * (1 - ((float)timeElapsed / SHUT_DOWN_TIME));
        motors[motorIndex]->sendRequest(TORQUE, torque);
    }
}



void MotorHandler::setMotorState(bool state)
{
    if (xSemaphoreTake(stateMutex, portMAX_DELAY) == pdTRUE) {
        this->motorOn = state;
        xSemaphoreGive(stateMutex);
    }
}


