#include "exoSettings.h"

ExoSettings *ExoSettings::instance;

ExoSettings::ExoSettings(){
    currentState = EnumExoStates::DISARMED;
    state = false;

    // 5 Pied 10
    height = 1.778;

    // Valeur arbitraire - TODO : Changer la valeur
    futureMotorPower = 0;
    currentMotorPower = 175;
}

ExoSettings& ExoSettings::getInstance(){
    if(instance == NULL){
        instance = new ExoSettings();
    }
    return *instance;
}

EnumExoStates ExoSettings::getCurrentState(){
    return currentState;
}

bool ExoSettings::getState(){
    return state;
}

int ExoSettings::getHeight(){
    return height;
}

int ExoSettings::getMotorPower(){
    return currentMotorPower;
}

void ExoSettings::setCurrentState(EnumExoStates newState){
    currentState = newState;
}

void ExoSettings::setState(bool nState){
    state = nState;
}

void ExoSettings::setHeight(int setHeight){
    height = setHeight;
}

void ExoSettings::setFutureMotorPower(double setFutureMotoPower){
    futureMotorPower = setFutureMotoPower;
}

void ExoSettings::saveMotorPower(){
    currentMotorPower = futureMotorPower;
}