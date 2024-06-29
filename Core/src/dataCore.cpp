#include "dataCore.h"

DataCore *DataCore::instance;

DataCore::DataCore(){
    motorEnabled = false;
    clutchEnabled = false;
    proximEnabled = true;
    brightness = HIGH;
    needResetProxim = false;

    sonarState = SQUAT_MODE;
    angleSource = FROM_IMU;

    height = 180;
    motorPower = 2048;
}

DataCore& DataCore::getInstance(){
    if(instance == NULL){
        instance = new DataCore();
    }
    return *instance;
}

void DataCore::init(QuadratureEncoder *encodeur){
    encodeurPtr = encodeur;
}

bool DataCore::isMotorEnabled(){
    return motorEnabled;
}

bool DataCore::isClutchEnabled(){
    return clutchEnabled;
}

bool DataCore::isProximEnabled(){
    return proximEnabled;
}

void DataCore::setMotorEnabled(bool setMotorEnabled){
    motorEnabled = !(motorEnabled);
    Serial.print("\t Motor enable set to: ");
    Serial.println(motorEnabled);
}

void DataCore::setClutchEnabled(bool setClutchEnabled){
    clutchEnabled = !(clutchEnabled);
    relais.setAllRelais(clutchEnabled);
    Serial.print("\t Clutch enable set to: ");
    Serial.println(clutchEnabled);
}

void DataCore::setProximEnabled(bool setProximEnabled){
    proximEnabled = !(proximEnabled);
    Serial.print("\t Proxim enable set to: ");
    Serial.println(proximEnabled);
}

bool DataCore::getAngleSource(){
    return angleSource;
}

void DataCore::setAngleSource(bool setAngleSource){
    angleSource = !(angleSource);
    Serial.print("\t Angle source set to: ");
    Serial.println(angleSource);
}

bool DataCore::getSonarState(){
    return sonarState;
}

void DataCore::setSonarState(bool setSonarState){
    sonarState = !(sonarState);
    Serial.print("\t Sonar state set to: ");
    Serial.println(sonarState);
}

int DataCore::getHeight(){
    return height;
}

void DataCore::setHeight(int setHeight){
    height = setHeight;
}

bool DataCore::getBrightness(){
    return brightness;
}

void DataCore::setBrightness(){
    brightness = !(brightness);
}

bool DataCore::getResetProxim(){
    return needResetProxim;
}

void DataCore::setResetProxim(bool reset){
    Serial.println("\t proxim reset to high ");
    needResetProxim = reset;
}

int DataCore::getMotorPower(){
    return motorPower;
}

void DataCore::setMotorPower(int newMotorPower){
    motorPower = float(newMotorPower) / 100.0 * 4096.0;
    Serial.print("\t Motor power set to: ");
    Serial.println(motorPower);
}

void DataCore::resetEncoder(){
    Serial.println("\t Reseting encoder ");
    encodeurPtr->resetPosition(QuadratureEncoder::GEN_DRO);
    encodeurPtr->resetPosition(QuadratureEncoder::GEN_GAU);
    encodeurPtr->resetPosition(QuadratureEncoder::HAN_DRO);
    encodeurPtr->resetPosition(QuadratureEncoder::HAN_GAU);
}

void DataCore::adjustMotorPower(int offset){
    setMotorPower(motorPower+offset);
}

void DataCore::initialise(){
    clutchEnabled = OFF;
    motorEnabled = false;
    motorPower = 2048;
    needResetProxim = true;
    sonarState = FROM_IMU;
    angleSource = FROM_IMU;
    resetEncoder();
}
