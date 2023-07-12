#include "multiplex.h"

Multiplex::Multiplex(){
    currentChannel = 0;
    selectChannel(0);
}

bool Multiplex::selectChannel(uint8_t channel){
    if(channel > 7 || channel < 0){
        return false;
    }

    Wire.beginTransmission(0x70);
    Wire.write(1 << channel);
    Wire.endTransmission();

    this->currentChannel = channel;

    return true;
}

int Multiplex::getCurrentChannel(){
    return currentChannel;
}


