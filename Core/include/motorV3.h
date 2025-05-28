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
};

#endif