#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <Arduino.h>
#include "motor.h"
#include "motorV2.h"
#include "motorV3.h"

enum class EnumMotorPosition
{
    KNEE_L = 0,
    KNEE_R = 1,
    HIP_L  = 2,
    HIP_R  = 3
};

class MotorHandler
{
private:
    enum EnumMotorPosition motorPos;
    SemaphoreHandle_t stateMutex;
    bool motorOn = true;

    //TODO : verifier les IDs
    Motor* motors[NB_MOTORS];
    bool initialized = false;
    
    //variables pour le slow shut down
    bool tempTooHigh = true;
    unsigned long shutdownStartTime = 0;

    float movingAverage[NB_MOTORS][SAMPLE_COUNT] = {0};
    float initialShutdownTorque[NB_MOTORS] = {0};

    void applyTorque(const float torque[NB_MOTORS]);
    void exitMotors();
    void slowShutDown();

    //! Things to add  !//
    // -Pin setup 
    // -Can setup 

public:
    MotorHandler();
    ~MotorHandler();

    void setMotorState(bool state);
    void initializeMotors();
    void Update(const float torque[NB_MOTORS]);
};

#endif