#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include <Arduino.h>
#include "motor.h"
#include "motorV2.h"
#include "motorV3.h"
#include "ESP32-TWAI-CAN.hpp"

namespace debug = app::config::debug;
namespace motor_config = app::config::motors;


class MotorHandler
{
private:

    SemaphoreHandle_t stateMutex;
    bool motorOn = true;

    CanFrame msg;

    //TODO : verifier les IDs
    Motor* motors[motor_config::amount];
    bool initialized = false;
    
    //variables pour le slow shut down
    bool tempTooHigh = true;
    unsigned long shutdownStartTime = 0;

    float movingAverage[motor_config::amount][motor_config::moving_avg_size] = {0};
    float initialShutdownTorque[motor_config::amount] = {0};

    void applyTorque(const float torque[motor_config::amount]);
    void exitMotors();
    void slowShutDown();

    void readCanReplyBuffer();

    //! Things to add  !//
    // -Pin setup 
    // -Can setup 

public:
    MotorHandler();
    ~MotorHandler();

    void setMotorState(bool state);
    void initializeMotors();
    void update(const float torque[motor_config::amount]);
};

#endif