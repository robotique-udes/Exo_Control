#include "exoSettings.h"

ExoSettings *ExoSettings::instance;

ExoSettings::ExoSettings(){
    motorEnabled = false;
    clutchEnabled = false;
    proximEnabled = true;

    sonarState = EnumSonarState::SQUAT;
    angleSource = EnumAngleSource::IMU;

    height = 180;
    motorPower = 50;
}

ExoSettings& ExoSettings::getInstance(){
    if(instance == NULL){
        instance = new ExoSettings();
    }
    return *instance;
}

bool ExoSettings::isMotorEnabled(){
    return motorEnabled;
}

bool ExoSettings::isClutchEnabled(){
    return clutchEnabled;
}

bool ExoSettings::isProximEnabled(){
    return proximEnabled;
}

void ExoSettings::setMotorEnabled(bool setMotorEnabled){
    motorEnabled = setMotorEnabled;
}

void ExoSettings::setClutchEnabled(bool setClutchEnabled){
    clutchEnabled = setClutchEnabled;
}

void ExoSettings::setProximEnabled(bool setProximEnabled){
    proximEnabled = setProximEnabled;
}

EnumAngleSource ExoSettings::getAngleSource(){
    return angleSource;
}

void ExoSettings::setAngleSource(EnumAngleSource setAngleSource){
    angleSource = setAngleSource;
}

EnumSonarState ExoSettings::getSonarState(){
    return sonarState;
}

void ExoSettings::setSonarState(EnumSonarState setSonarState){
    sonarState = setSonarState;
}

int ExoSettings::getHeight(){
    return height;
}

void ExoSettings::setHeight(int setHeight){
    height = setHeight;
}

int ExoSettings::getMotorPower(){
    return motorPower;
}

void ExoSettings::setMotorPower(int setMotorPower){
    motorEnabled = setMotorPower;
}
