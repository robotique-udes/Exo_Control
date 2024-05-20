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

EnumExoStates ExoSettings::getMotorState(){
    return currentState;
}

bool ExoSettings::getProximState(){
    return state;
}

int ExoSettings::getHeight(){
    return height;
}

int ExoSettings::getMotorPower(){
    return currentMotorPower;
}

void ExoSettings::setMotorState(EnumExoStates newState){
    currentState = newState;
    
}

void ExoSettings::setProximState(bool nState){
    state = nState;
    Serial.print("Logic state: ");
    Serial.println(state);
}

void ExoSettings::setHeight(int setHeight){
    height = setHeight;
    Serial.print("Height: ");
    Serial.println(height);
}

void ExoSettings::setFutureMotorPower(double setFutureMotoPower){
    futureMotorPower = setFutureMotoPower;
    Serial.println(futureMotorPower);
}

void ExoSettings::saveMotorPower(){
    currentMotorPower = futureMotorPower;
}