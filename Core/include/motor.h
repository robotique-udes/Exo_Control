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
protected:
    //DataCore instanciation
    DataCore& dataCore = DataCore::getInstance();
    CanFrame msg;
    float temperature; 
    int motorId;
    enum EnumMotorPosition motorPosition;
    float currentTorque;

public:
    Motor();
    Motor(int id);
    ~Motor();
    

    void sendRequest(MotorMode mode, float value);

    int getTemperature();   

    void setMotorId(int id);

    int getMotorId();

    enum EnumMotorPosition getMotorPosition();

    float getCurrentTorque();

    void setID(int id);

};

#endif