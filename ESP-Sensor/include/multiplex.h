#ifndef MULTIPLEX_H_
#define MULTIPLEX_H_
#include <Wire.h>

class Multiplex{

public:
    const static uint8_t MUX_ADDR = 0x70;

    Multiplex();
    // Default constructor
    Multiplex(TwoWire* wireUsed);
    // Selects the channel
    bool selectChannel(uint8_t channel);
    // Returns the current active port
    int getCurrentChannel();
    TwoWire* getWire();

private:
    TwoWire* monWire;
    uint8_t currentChannel;
};

#endif