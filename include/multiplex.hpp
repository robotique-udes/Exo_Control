#ifndef MULTIPLEX_HPP
#define MULTIPLEX_HPP
#include <Wire.h>
#include "config.hpp"

class Multiplex{

public:
    //Mux address
    const static uint8_t MUX_ADDR = 0x70;

    Multiplex();

    /**
     * @brief Default constructor
     * @param wireUsed Pointer to twoWire instance
     */
    Multiplex(TwoWire* wireUsed);

    /**
     * @brief Selects mux channel
     * @param channel Target channel
     * @return Success
     */
    bool selectChannel(uint8_t channel);

    /**
     * @brief Returns the current active port
     * @return Active port
     */
    int getCurrentChannel();

private:
    //twoWire instance
    TwoWire* monWire;
    //Active channel
    uint8_t currentChannel;
};

#endif