#ifndef SENSOROPT3101_H
#define SENSOROPT3101_H

#include <OPT3101.h>
#include <Wire.h>
#include <Arduino.h>
#include "multiplex.h"
#include "define.h"
#include <algorithm>

#define GROUND_DISTANCE_RANGE 60

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
    std::fill_n(bufferOnTheGround, BUFFER_SIZE, 1);
    

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
    void SetTriggerDistance();
    
};

#endif