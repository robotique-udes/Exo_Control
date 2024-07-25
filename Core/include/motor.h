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
    //DataCore instanciation
    DataCore& dataCore = DataCore::getInstance();

public:
    Motor();
    ~Motor();

    /**
     * @brief Pin initialisation
     */
    void setPins();

    /**
     * @brief Send PWM value to motors by calling motor speed for every motor
     */
    void write();

    /**
     * @brief Send PWM value to a specific motor
     * @param ID motor ID ranging from 0 to 3
     * @param val PWM value from -4096 to 4096
     */
    void motorSetSpeed(int ID, int val);

    /**
     * @brief Test function for I don't know what, ask Edouard
     */
    void testFuncSetPwnHigh();
};

#endif