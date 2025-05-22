#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H



#include <Arduino.h>
#include "enums.h"
#include "motor.h"
#include "dataCore.h"

class MotorHandler
{
private:
    Motor motors[NB_MOTORS];
    DataCore &dataCore = DataCore::getInstance();

    //variables pour le slow shut down
    bool tempTooHigh = false;
    unsigned long previousTime = 0;
    float totalTime = 0;
    float initialTorque[4] = {0, 0, 0, 0};

    void applyTorque();
    void initializeMotors();
    void exitMotors();
    void slowShutDown();

public:
    MotorHandler();
    ~MotorHandler();

    void Update();
};

#endif