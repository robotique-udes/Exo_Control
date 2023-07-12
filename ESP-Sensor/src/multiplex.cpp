#include "multiplex.h"

Multiplex::Multiplex(){}

Multiplex::Multiplex(TwoWire* wireUsed){
    this->monWire = wireUsed;
    currentChannel = 0;
    selectChannel(0);
}

bool Multiplex::selectChannel(uint8_t channel){
    if(channel > 7 || channel < 0){
        return false;
    }

    monWire->beginTransmission(0x70);
    monWire->write(1 << channel);
    monWire->endTransmission();

    this->currentChannel = channel;

    return true;
}

int Multiplex::getCurrentChannel(){
    return currentChannel;
}

