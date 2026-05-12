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

    void packCmd(float position, float velocity, float kp, float kd, float torque);

    void unpackReply(CanFrame msgReply);

    void sendCommand(MotorMode mode,float value); 


};

#endif