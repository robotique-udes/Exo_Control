#include "exoSettings.h"

ExoSettings *ExoSettings::instance;

ExoSettings::ExoSettings(){
    motorEnabled = false;
    clutchEnabled = false;
    proximEnabled = true;

    sonarState = SQUAT_MODE;
    angleSource = FROM_IMU;

    height = 180;
    motorPower = 50;
}

ExoSettings& ExoSettings::getInstance(){
    if(instance == NULL){
        instance = new ExoSettings();
    }
    return *instance;
}

void ExoSettings::init(QuadratureEncoder *encodeur){
    encodeurPtr = encodeur;
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
    motorEnabled = !(motorEnabled);
    Serial.print("\t Motor enable set to: ");
    Serial.println(motorEnabled);
}

void ExoSettings::setClutchEnabled(bool setClutchEnabled){
    clutchEnabled = !(clutchEnabled);
    relais.setAllRelais(clutchEnabled);
    Serial.print("\t Clutch enable set to: ");
    Serial.println(clutchEnabled);
}

void ExoSettings::setProximEnabled(bool setProximEnabled){
    proximEnabled = !(proximEnabled);
    Serial.print("\t Proxim enable set to: ");
    Serial.println(proximEnabled);
}

bool ExoSettings::getAngleSource(){
    return angleSource;
}

void ExoSettings::setAngleSource(bool setAngleSource){
    angleSource = !(angleSource);
    Serial.print("\t Angle source set to: ");
    Serial.println(angleSource);
}

bool ExoSettings::getSonarState(){
    return sonarState;
}

void ExoSettings::setSonarState(bool setSonarState){
    sonarState = !(sonarState);
    Serial.print("\t Sonar state set to: ");
    Serial.println(sonarState);
}

int ExoSettings::getHeight(){
    return height;
}

void ExoSettings::setHeight(int setHeight){
    height = setHeight;
}

void ExoSettings::setBrightness(){}

bool ExoSettings::getResetProxim(){
    return needResetProxim;
}

void ExoSettings::setResetProxim(bool reset){
    Serial.println("\t proxim reset to high ");
    needResetProxim = reset;
}

int ExoSettings::getMotorPower(){
    return motorPower;
}

void ExoSettings::setMotorPower(int setMotorPower){
    motorPower = setMotorPower;
    Serial.print("\t Motor power set to: ");
    Serial.println(motorEnabled);
}

void ExoSettings::resetEncoder(){
    Serial.println("\t Reseting encoder ");
    encodeurPtr->resetPosition(QuadratureEncoder::GEN_DRO);
    encodeurPtr->resetPosition(QuadratureEncoder::GEN_GAU);
    encodeurPtr->resetPosition(QuadratureEncoder::HAN_DRO);
    encodeurPtr->resetPosition(QuadratureEncoder::HAN_GAU);
}

void ExoSettings::adjustMotorPower(int offset){
    setMotorPower(motorPower+offset);
}
