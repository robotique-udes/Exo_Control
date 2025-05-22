#ifndef MOTORV2_H
#define MOTORV2_H
#include "motor.h"

class MotorV2 : public Motor
{

public :
    MotorV2();

    ~MotorV2();

    void applyTorque(MotorMode mode, float value);

//private: for test now, can uncomment later

    /**
     * @brief Pin initialisation
     */
    void setPins();

    void sendCanMessage(CanFrame* msg);

    void receiveCanMessage(CanFrame* msg);

    void enterMode();

    void exitMode();

    void zeroSet();

    void packCmd(float position, float velocity, float kp, float kd, float torque);

    void unpackReply();

    void sendCommand(MotorMode mode,float value); 

    unsigned int float_to_uint(float x, float x_min, float x_max, int bits);

    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);

};

#endif