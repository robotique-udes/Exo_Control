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
    float currentTorque[NB_MOTORS];
    bool tempTooHigh = false;
    DataCore &dataCore = DataCore::getInstance();

    unsigned long previousTime = 0;


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