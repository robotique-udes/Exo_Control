#ifndef MOTOR_HANDLER_H
#define MOTOR_HANDLER_H

#include "config.h"
#include "cubemarsAK10-9KV100V2.h"
#include "cubemarsAK10-9KV60V3.h"

namespace debug = app::config::debug;
namespace motor_config = app::config::motors;


class MotorHandler
{
private:

    SemaphoreHandle_t stateMutex;
    bool motorOn = true;

    CanFrame msg;

    //TODO : verifier les IDs
    IMitModeMotor* motors[motor_config::amount];
    Cubemars_AK10_9_KV100_V2 kneeLeftMotor = Cubemars_AK10_9_KV100_V2(motor_config::knee_left);
    Cubemars_AK10_9_KV100_V2 kneeRightMotor = Cubemars_AK10_9_KV100_V2(motor_config::knee_right);
    Cubemars_AK10_9_KV60_V3 hipLeftMotor = Cubemars_AK10_9_KV60_V3(motor_config::hip_left);
    Cubemars_AK10_9_KV60_V3 hipRightMotor = Cubemars_AK10_9_KV60_V3(motor_config::hip_right);
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