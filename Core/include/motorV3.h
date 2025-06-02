#ifndef MOTORV3_H
#define MOTORV3_H
#include "motor.h"


//TODO - implementer le code du V3
class MotorV3 : public Motor
{
public:
    MotorV3();
    MotorV3(int id);
    ~MotorV3();
    

    void sendRequest(MotorMode mode, float value);

    void sendCanMessage(CanFrame* msg);

    void receiveCanMessage(CanFrame* msg);

    void packCmd(float torque);

    void unpackReply();

    void sendCommand(MotorMode mode,float value); 

    unsigned int float_to_uint(float x, float x_min, float x_max, int bits);

    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);

};

#endif