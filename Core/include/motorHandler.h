#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <Arduino.h>
#include "motor.h"
#include "motorV2.h"
#include "motorV3.h"

enum class EnumMotorPosition
{
    HIP_R  = 0,
    HIP_L  = 1,
    KNEE_R = 2,
    KNEE_L = 3
};

class MotorHandler
{
private:
    enum EnumMotorPosition motorPos;

    //TODO : verifier les IDs
    Motor motors[NB_MOTORS] = {MotorV3(static_cast<int>(EnumMotorPosition::HIP_R)), 
                               MotorV2(static_cast<int>(EnumMotorPosition::HIP_L)),
                               MotorV3(static_cast<int>(EnumMotorPosition::KNEE_R)), 
                               MotorV2(static_cast<int>(EnumMotorPosition::KNEE_L))};

    //variables pour le slow shut down
    bool tempTooHigh = false;
    unsigned long previousTime = 0;
    float totalTime = 0;
    float initialTorque[4] = {0, 0, 0, 0};

    void applyTorque();
    void initializeMotors();
    void exitMotors();
    void slowShutDown();

    //! Things to add  !//
    // -Pin setup 
    // -Can setup 

public:
    MotorHandler();
    ~MotorHandler();

    void Update();
};

#endif