#ifndef MULTIPLEX_H
#define MULTIPLEX_H

#include <Wire.h>

class Multiplex{

public:
    // Default constructor
    Multiplex();
    // Selects the channel
    bool selectChannel(uint8_t channel);
    // Returns the current active port
    int getCurrentChannel();

private:
    
    TwoWire* monWire;
    int currentChannel;
};

#endif