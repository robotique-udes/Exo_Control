#include "biogeniusImu.h"
#include "enumIMU.h"

IMU::IMU(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B, back will also use 0x4B
    // Left BNOs are on multiplex channel 1, right BNOs are on multiplex channel 2
    BNOs.push_back(new BNO(enumIMU.HipL, 1, &this->mux, 0x4A));
}