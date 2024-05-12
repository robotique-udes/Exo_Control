#include "biogeniusImu.h"
#include "enum.h"

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

IMU::begin(){
    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->begin();
        Serial.println("IMU " + i + " initialized");
    }
}

IMU::requestData(){
    bool showTime = false;
    if (showTime){
        Serial.println("Time since last update: " + (millis() - last_update) + "ms");
    }

    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->requestData();
    }

    last_update = millis();
}

IMU::getValAngle(enumIMU position){
    Serial.println("\nIMU " + position + " angle: " + BNOs[position]->getEuler(true)[0];
    return BNOs[position]->getValAngle();
}

