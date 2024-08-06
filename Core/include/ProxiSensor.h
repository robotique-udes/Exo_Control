#ifndef PROXISENSOR_H
#define PROXISENSOR_H

#include <OPT3101.h>
#include <Wire.h>
#include <Arduino.h>
#include "multiplex.h"
#include "define.h"
#include "dataCore.h"


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

    /**
    * @brief Scan each 3 sensors to get the closest distance between the 3 using an average of 3 readings 
    * @return Closest distance to ground
    */
    int GetMinDistance();
    /**
    * @brief Update new min distance in a buffer, and use an average of every value in said buffer to determine if foot is grounded 
    * @return Does foot has a contect on the ground
    */
    bool IsOnTheGround();
    /**
    * @brief Getter for current trigger distance (threshold) 
    * @return Trigger distance
    */
    int GetTriggerDistance();
    /**
    * @brief Update brightness and auto calibrate trigger distance with an average of 3 GetMinDistance() readings
    * @param brightness New brightness lvl
    */
    void SetTriggerDistance(bool brightness);
    /**
    * @brief Getter for current channel used by proxim's internal MUX 
    * @return Channel
    */
    int GetChannel();
    /**
    * @brief Setter for channel used by proxim's interne MUX 
    * @param c Target channel
    */
    void SetChannel(int c);
    /**
    * @brief Print proxim's interne settings, plus it's last min distance read
    */
    void PrintData();
    /**
    * @brief Set open frame time, lower value increase reading speed but decrease accuracy
    * @param FrameTiming New frame timing, should be a power of 2
    */
    void SetFrameTiming(int FrameTiming);
    
    
};

#endif