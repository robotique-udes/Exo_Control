#ifndef SENSOROPT3101_H
#define SENSOROPT3101_H

#include <OPT3101.h>
#include <Wire.h>
#include <Arduino.h>

#define GROUND_DISTANCE_RANGE 60

class SensorOPT3101
{
private:
    OPT3101 CoreSensor;
    bool OnTheGround= false;
    int TriggerDistance = 0;
    int minimumDistance = 0;

public:
    SensorOPT3101(int FrameTiming=128, int sda=26, int scl=27);

    int GetMinDistance();
    bool IsOnTheGround();
    int GetTriggerDistance();
    int getChannel();
    void printData();
    void printDistance();
    void SetFrameTiming(int FrameTiming);
    void SetTriggerDistance();
    
};

#endif