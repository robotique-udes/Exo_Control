#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "define.h"
#include "ESP32-TWAI-CAN.hpp"


enum MotorMode
{
    TORQUE = 1,
    VELOCITY = 2,
    POSITION = 3
};
class Motor
{
protected:
    //DataCore instanciation
    CanFrame msg;
    float temperature = 0.0; 
    int motorId;
    float currentTorque;
    float motorCorrectionSlope;
    float motorCorrectionOffset;
    bool motorOn = false;

public:
    Motor();
    Motor(int id);
    ~Motor();
    

    virtual void sendRequest(MotorMode mode, float value) = 0;

    virtual void start();
    
    virtual void stop();

    virtual void unpackReply(CanFrame msgReply);

    void sendCanMessage(CanFrame* msg);

    void receiveCanMessage(CanFrame* msg);

    int getTemperature();  
    
    void setTemperature(int temperature);

    void setMotorId(int id);

    int getMotorId();

    void setMotorCorrection(float slope, float offset);

    float getMotorCorrectionSlope();

    float getMotorCorrectionOffset();

    float getCurrentTorque();

    void setMotorState(bool motorOn);

    unsigned int float_to_uint(float x, float x_min, float x_max, int bits);

    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);

};

#endif