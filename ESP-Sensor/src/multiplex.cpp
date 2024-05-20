#include "multiplex.h"
#include <Wire.h>
#include "Arduino.h"
#include "define.h"

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
    // TODO don't write if already on good channel

    monWire->beginTransmission(this->MUX_ADDR);
    monWire->write(1 << channel);
    monWire->endTransmission();

    this->currentChannel = channel;

    return true;
}

int Multiplex::getCurrentChannel(){
    return currentChannel;
}

TwoWire* Multiplex::getWire(){
    return monWire;
}

