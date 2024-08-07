#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "define.h"
#include "PinExtender.h"

class Relay
{
public: 

    /**
    * @brief Set output pins
    */
    void setPins();

    /**
    * @brief Set specific relay on/off
    * @param ID Target relay id
    * @param state Target state
    */
    void setRelay(int ID, bool state);

    /**
    * @brief Set all relay to target state
    * @param state Target state
    */
    void setAllRelay(bool state);
};

#endif