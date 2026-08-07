/**
 * @file Multiplex.cpp
 * @brief Implementation of the Multiplex class
 * 
 * @author Eloi Charbonneau
 * @date 2026-07-18
 */

#include "Multiplex.hpp"
#include <Wire.h>
#include "Arduino.h"
#include "config.hpp"

Multiplex::Multiplex(){
    this->monWire = &Wire;
    selectChannel(0);
}

Multiplex::Multiplex(TwoWire* wireUsed){
    this->monWire = wireUsed;
    selectChannel(0);
}

bool Multiplex::selectChannel(uint8_t channel){
    if(channel > 7 || channel < 0){
        return false;
    }
    /// @todo don't write if already on good channel
    monWire->beginTransmission(this->MUX_ADDR);
    monWire->write(1 << channel);
    monWire->endTransmission();

    this->currentChannel = channel;

    return true;
}

int Multiplex::getCurrentChannel(){
    return currentChannel;
}
