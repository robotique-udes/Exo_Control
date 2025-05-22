#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "enums.h"
#include "define.h"
#include "PinExtender.h"
#include "dataCore.h"
#include "multiplex.h"
#include "ESP32-TWAI-CAN.hpp"

class Motor
{
private:
    //DataCore instanciation
    DataCore& dataCore = DataCore::getInstance();
    CanFrame msg;
    float temperature; 
    int motorId;
    enum EnumMotorPosition motorPosition;
    float currentTorque;

public:
    Motor();
    ~Motor();

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

    int getTemperature();   

    void setMotorId(int id);

    int getMotorId();

    unsigned int float_to_uint(float x, float x_min, float x_max, int bits);

    float uint_to_float(unsigned int x_int, float x_min, float x_max, int bits);

    enum EnumMotorPosition getMotorPosition();

    float getCurrentTorque();

};

#endif