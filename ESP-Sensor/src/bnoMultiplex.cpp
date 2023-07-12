#include "bnoMultiplex.h"

BNOMultiplex::BNOMultiplex(int32_t sensorID, uint8_t address, TwoWire* theWire, uint8_t channel){
    _monMultiplex = Multiplex(theWire);
    _channel = channel;
    Adafruit_BNO055(sensorID, address, theWire);
}

bool BNOMultiplex::begin(adafruit_bno055_opmode_t mode){
    _monMultiplex.selectChannel(_channel);
    return Adafruit_BNO055::begin(mode);
}

void BNOMultiplex::setExtCrystalUse(boolean usextal){
    _monMultiplex.selectChannel(_channel);
    Adafruit_BNO055::setExtCrystalUse(usextal);
}

imu::Quaternion BNOMultiplex::getQuat(){
    _monMultiplex.selectChannel(_channel);
    return Adafruit_BNO055::getQuat();
}