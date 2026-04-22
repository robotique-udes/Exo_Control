#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "enums.h"
#include "define.h"
#include "ESP32-TWAI-CAN.hpp"

class Motor
{
protected:
    //DataCore instanciation
    CanFrame msg;
    float temperature; 
    int motorId;
    enum EnumMotorPosition motorPosition;
    float currentTorque;
    float motorCorrectionSlope;
    float motorCorrectionOffset;

public:
    Motor();
    Motor(int id);
    ~Motor();
    

    void sendRequest(MotorMode mode, float value);

    void sendCanMessage(CanFrame* msg);

    void receiveCanMessage(CanFrame* msg);

    int getTemperature();   

    void setMotorId(int id);

    int getMotorId();

    void setMotorCorrection(float slope, float offset);

    float getMotorCorrectionSlope();

    float getMotorCorrectionOffset();

    enum EnumMotorPosition getMotorPosition();

    float getCurrentTorque();

    unsigned int float_to_uint(float x, float x_min, float x_max, int bits);

    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);

};

#endif