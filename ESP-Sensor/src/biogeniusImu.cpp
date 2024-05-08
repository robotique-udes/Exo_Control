#include "biogeniusImu.h"
#include "enumIMU.h"

IMU::IMU(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs.push_back(new BNO(enumIMU.HipL, 1, &this->mux, 0x4A));
    BNOs.push_back(new BNO(enumIMU.KneeL, 1, &this->mux, 0x4B));
    BNOs.push_back(new BNO(enumIMU.HipR, 2, &this->mux, 0x4A));
    BNOs.push_back(new BNO(enumIMU.KneeR, 2, &this->mux, 0x4B));
    BNOs.push_back(new BNO(enumIMU.Back, 0, &this->mux, 0x4A));
}

