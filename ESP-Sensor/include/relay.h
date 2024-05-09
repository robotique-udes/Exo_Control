#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "define.h"
#include "PinExtender.h"

class Relay
{
public: 
    void setPins();
    void setRelais(int ID, bool state);
    void setAllRelais(bool state);
};

#endif