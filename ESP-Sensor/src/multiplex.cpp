#include "multiplex.h"
#include <Wire.h>
#include "Arduino.h"

Multiplex::Multiplex(){
    Serial.println("yooooo");
    this->monWire = &Wire;
    currentChannel = 0;
    bool err = selectChannel(0);
    Serial.print("Multiplex error:");
    Serial.println(err);
    // while(true){}
}

Multiplex::Multiplex(TwoWire* wireUsed){
    this->monWire = wireUsed;
    currentChannel = 0;
    selectChannel(0);
}

bool Multiplex::selectChannel(uint8_t channel){
    if(channel > 7 || channel < 0){
        return false;
    }

    Serial.printf("wire is available: %d", monWire->available());// le wire est pas available faque beginTransmission est pas content
    // while(true){}
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

