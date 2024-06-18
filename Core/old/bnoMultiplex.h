#ifndef BNOMULTIPLEX_H_
#define BNOMULTIPLEX_H_

#include <Wire.h>
#include <Adafruit_BNO055.h>
#include "multiplex.h"


class BNOMultiplex : public Adafruit_BNO055
{

public:

    BNOMultiplex(int32_t sensorID, uint8_t address, TwoWire *theWire, uint8_t channel);

    bool begin(adafruit_bno055_opmode_t mode = OPERATION_MODE_NDOF);

    void setExtCrystalUse(boolean usextal);

    imu::Quaternion getQuat();

private:
    uint8_t _channel;
    Multiplex _monMultiplex;

};

#endif