#ifndef SENSOROPT3101_H
#define SENSOROPT3101_H

#include <OPT3101.h>
#include <Wire.h>
#include <Arduino.h>
#include "multiplex.h"
#include "define.h"


class ProxiSensor
{
private:
    OPT3101 CoreSensor;
    bool OnTheGround= false;
    int TriggerDistance = 0;
    int minimumDistance = 0;
    int muxAddress;
    Multiplex *muxPtr;
    int bufferOnTheGround[BUFFER_SIZE];

public:
    ProxiSensor(Multiplex *muxPtr, int muxAddress);

    int GetMinDistance();
    bool IsOnTheGround();
    int GetTriggerDistance();
    int GetChannel();
    void SetChannel(int c);
    void PrintData();
    void PrintDistance();
    void SetFrameTiming(int FrameTiming);
    void SetTriggerDistance(bool brightness);
    
};

#endif