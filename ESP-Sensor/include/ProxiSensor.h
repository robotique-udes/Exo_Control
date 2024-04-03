#ifndef SENSOROPT3101_H
#define SENSOROPT3101_H

#include <OPT3101.h>
#include <Wire.h>
#include <Arduino.h>

#define GROUND_DISTANCE_RANGE 60

class ProxiSensor
{
private:
    OPT3101 CoreSensor;
    bool OnTheGround= false;
    int TriggerDistance = 0;
    int minimumDistance = 0;

public:
    ProxiSensor(int FrameTiming=128, int sda=26, int scl=27);

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