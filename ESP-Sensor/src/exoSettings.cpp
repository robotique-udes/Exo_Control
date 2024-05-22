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

bool ExoSettings::getAngleSource(){
    return angleSource;
}

int ExoSettings::getHeight(){
    return height;
}

int ExoSettings::getMotorPower(){
    return currentMotorPower;
}

void ExoSettings::setMotorState(EnumExoStates newState){
    currentState = newState;
    switch(newState)
    {
    case EnumExoStates::DISARMED:
        relais.setAllRelais(OFF);
        break;
    case EnumExoStates::ARMED:
        relais.setAllRelais(ON);
        break;
    default:
        break;
    }
    
}

void ExoSettings::setProximState(bool newState){
    state = newState;
    Serial.print("Proxim state: ");
    Serial.println(state);
}

void ExoSettings::setAngleSource(bool newSource){
    angleSource = newSource;
    Serial.print("Angle source: ");
    Serial.println(angleSource);
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

