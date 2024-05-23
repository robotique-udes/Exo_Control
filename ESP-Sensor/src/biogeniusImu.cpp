#include "biogeniusImu.h"
#include "Arduino.h"
using namespace std;

IMU::IMU(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (enumIMU::HIP_L)]     = new BNO(enumIMU::HIP_L, 1, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_L)]    = new BNO(enumIMU::KNEE_L, 1, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::HIP_R)]     = new BNO(enumIMU::HIP_R, 2, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_R)]    = new BNO(enumIMU::KNEE_R, 2, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::EXO_BACK)]  = new BNO(enumIMU::EXO_BACK, 0, &this->mux, 0x4A);
}

// OPTIONAL: Check if a BNO is connected
bool IMU::begin(){
    int8_t connected = 0;
    if (IMU_DEBUG) Serial.print("===== IMU STARTING =====\n");
    for (int i = 0; i < BNOs.size(); i++){

        if (BNOs[i]->checkIfConnected()){
            connected++;
        }

        if (IMU_DEBUG) {
            Serial.print("IMU "); Serial.print(i); Serial.print("\t");
            Serial.print("Connected: "); Serial.println(BNOs[i]->checkIfConnected());
            delay(500);
        }
    }
    // At lease one BNO is up and running
    return connected > 0;
}

// Request data from all BNOs
void IMU::requestData(){
    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->requestData();
    }
    this->computeAngles();

    last_update = millis();
}

// Print relevant IMU information
void IMU::printBNOs(int startIndex, int endIndex){
    Serial.print("\tIMU "); BNOs[i]->printName(); Serial.print("\t");
    Serial.print("LINK: "); Serial.print(BNOs[i]->checkIfConnected());
    Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<enumIMU>(i)));
}

void IMU::computeAngles() {
    float hipL = BNOs[static_cast<int> (enumIMU::HIP_L)]->getEuler()[1];
    float kneeL = -BNOs[static_cast<int> (enumIMU::KNEE_L)]->getEuler()[1];
    float hipR = -BNOs[static_cast<int> (enumIMU::HIP_R)]->getEuler()[1];
    float kneeR = BNOs[static_cast<int> (enumIMU::KNEE_R)]->getEuler()[1];
    float back = BNOs[static_cast<int> (enumIMU::EXO_BACK)]->getEuler()[2];

    angles[static_cast<int> (enumIMU::HIP_L)] = hipL;
    angles[static_cast<int> (enumIMU::KNEE_L)] = abs(kneeL-hipL);

    angles[static_cast<int> (enumIMU::HIP_R)] = hipR;
    angles[static_cast<int> (enumIMU::KNEE_R)] = abs(kneeR-hipR);

    angles[static_cast<int> (enumIMU::EXO_BACK)] = -(back-90);

}

// Returns Yaw, same as previous implementation
float IMU::getValAngle(enumIMU position){
    int pos = static_cast<int> (position);
    return this->angles[pos];
}

BNOStruct IMU::getBNOData(enumIMU position){
    return BNOs[static_cast<int> (position)]->getData();
}
