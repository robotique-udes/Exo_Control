#include "bnoHandler.h"
#include "Arduino.h"
#include "define.h"
using namespace std;

BnoHandler::BnoHandler(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (EnumBnoPosition::HIP_L)]     = new BNO_085(EnumBnoPosition::HIP_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (EnumBnoPosition::KNEE_L)]    = new BNO_085(EnumBnoPosition::KNEE_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
    BNOs[static_cast<int> (EnumBnoPosition::HIP_R)]     = new BNO_085(EnumBnoPosition::HIP_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (EnumBnoPosition::KNEE_R)]    = new BNO_085(EnumBnoPosition::KNEE_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
    BNOs[static_cast<int> (EnumBnoPosition::EXO_BACK)]  = new BNO_085(EnumBnoPosition::EXO_BACK, 0, &this->mux, 0x4A);
}

// OPTIONAL: Check if a BNO is connected
bool BnoHandler::begin(){
    int8_t connected = 0;
    if (IMU_DEBUG) Serial.print("===== BnoHandler STARTING =====\n");
    for (int i = 0; i < BNOs.size(); i++){

        if (BNOs[i]->checkIfConnected()){
            connected++;
        }

        if (IMU_DEBUG) {
            Serial.print("BNO "); Serial.print(i); Serial.print("\t");
            Serial.print("Connected: "); Serial.println(BNOs[i]->checkIfConnected());
            delay(500);
        }
    }
    // At lease one BNO is up and running
    return connected > 0;
}

void BnoHandler::read(){
    requestData();
    dataCore.setBnoAngles(EnumBnoPosition::HIP_R, getValAngle(EnumBnoPosition::HIP_R));
    dataCore.setBnoAngles(EnumBnoPosition::HIP_L, getValAngle(EnumBnoPosition::HIP_L));
    dataCore.setBnoAngles(EnumBnoPosition::KNEE_R, getValAngle(EnumBnoPosition::KNEE_R));
    dataCore.setBnoAngles(EnumBnoPosition::KNEE_L, getValAngle(EnumBnoPosition::KNEE_L));
    dataCore.setBnoAngles(EnumBnoPosition::EXO_BACK, getValAngle(EnumBnoPosition::EXO_BACK));
}

// Request data from all BNOs
void BnoHandler::requestData(){
    for (int i = 0; i < BNOs.size(); i++){
        BNOs[i]->requestData();
    }
    this->computeAngles();

    last_update = millis();
}

// Print relevant IMU information
void BnoHandler::printBNOs(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        Serial.print("\tIMU "); BNOs[i]->printName(); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(BNOs[i]->checkIfConnected());
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<EnumBnoPosition>(i)));
    }
}

void BnoHandler::computeAngles() {
    float thighL = BNOs[static_cast<int> (EnumBnoPosition::HIP_L)]->getEuler()[1];
    float tibiaL = -BNOs[static_cast<int> (EnumBnoPosition::KNEE_L)]->getEuler()[1];
    float thighR = -BNOs[static_cast<int> (EnumBnoPosition::HIP_R)]->getEuler()[1];
    float tibiaR = BNOs[static_cast<int> (EnumBnoPosition::KNEE_R)]->getEuler()[1];

    // Compute back angle "right away" to be used in hip calculations
    float back = BNOs[static_cast<int> (EnumBnoPosition::EXO_BACK)]->getEuler()[1];

    angles[static_cast<int> (EnumBnoPosition::HIP_L)] = abs(-thighL-back);
    angles[static_cast<int> (EnumBnoPosition::KNEE_L)] = abs(tibiaL-thighL);

    angles[static_cast<int> (EnumBnoPosition::HIP_R)] = abs(-thighR-back);
    angles[static_cast<int> (EnumBnoPosition::KNEE_R)] = abs(tibiaR-thighR);

    angles[static_cast<int> (EnumBnoPosition::EXO_BACK)] = back;

}

// Returns Yaw, same as previous implementation (BNO_055 used on Darianne)
float BnoHandler::getValAngle(EnumBnoPosition position){
    int pos = static_cast<int> (position);
    return this->angles[pos];
}

BNOStruct BnoHandler::getBNOData(EnumBnoPosition position){
    return BNOs[static_cast<int> (position)]->getData();
}
