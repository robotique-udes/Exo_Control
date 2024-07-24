#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "enums.h"
#include "define.h"
#include "PinExtender.h"
#include "dataCore.h"
#include "ProxiSensor.h"
#include "multiplex.h"

class Motor
{
private:
    //settings
    DataCore& dataCore = DataCore::getInstance();

public:
    Motor();
    ~Motor();

    void setPins();
    void write();
    void motorSetSpeed(int ID, int val); // ID 0 to 3, val -4096 to 4096

    void testFuncSetPwnHigh();
};

#endif