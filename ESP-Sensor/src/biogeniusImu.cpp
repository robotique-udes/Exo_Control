#include "biogeniusImu.h"
#include "enum.h"
#include "Arduino.h"
using namespace std;

IMU::IMU(){
    Serial.println("qqch");
    // while(true){}
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (enumIMU::HIP_L)]     = new BNO(enumIMU::HIP_L, 1, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_L)]    = new BNO(enumIMU::KNEE_L, 1, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::HIP_R)]     = new BNO(enumIMU::HIP_R, 2, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_R)]    = new BNO(enumIMU::KNEE_R, 2, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::EXO_BACK)]  = new BNO(enumIMU::EXO_BACK, 0, &this->mux, 0x4A);
}

IMU::IMU(TwoWire* wireUsed){
    this->mux = Multiplex(wireUsed);
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (enumIMU::HIP_L)]     = new BNO(enumIMU::HIP_L, 1, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_L)]    = new BNO(enumIMU::KNEE_L, 1, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::HIP_R)]     = new BNO(enumIMU::HIP_R, 2, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_R)]    = new BNO(enumIMU::KNEE_R, 2, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::EXO_BACK)]  = new BNO(enumIMU::EXO_BACK, 0, &this->mux, 0x4A);
}

bool IMU::begin(){
    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->beginTracking();
        Serial.printf("OK IMU # %d\n", i);
    }
    return true;
}

void IMU::requestData(){
    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->requestData();
    }

    last_update = millis();
}

int16_t IMU::getValAngle(enumIMU position){
    int pos = static_cast<int> (position);
    Serial.print("IMU "); Serial.print(pos); Serial.print("\t"); 
    Serial.print("Euler:  ");   Serial.print("\t");
    
    Serial.print(BNOs[pos]->getGyro()[0]);    Serial.print("\t");
    Serial.print(BNOs[pos]->getGyro()[1]);    Serial.print("\t");
    Serial.print(BNOs[pos]->getGyro()[2]);    Serial.print("\t");
    Serial.println();

    return BNOs[pos]->getEuler()[0];
}

BNOStruct IMU::getBNOData(enumIMU position){
    return BNOs[static_cast<int> (position)]->getData();
}

