#ifndef MOTORV2_H
#define MOTORV2_H
#include "motor.h"

class MotorV2 : public Motor
{

public :
    MotorV2();
    MotorV2(int id);

    ~MotorV2();

void sendRequest(MotorMode mode, float value);

//private: for test now, can uncomment later

    /**
     * @brief Pin initialisation
     */
    void setPins();

    void enterMode();

    void exitMode();

    void zeroSet();

    void packCmd(float position, float velocity, float kp, float kd, float torque);

    void unpackReply();

    void sendCommand(MotorMode mode,float value); 

};

#endif