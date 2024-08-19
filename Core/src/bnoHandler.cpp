#include "bnoHandler.h"
#include "Arduino.h"
#include "define.h"
using namespace std;

BnoHandler::BnoHandler(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (EnumBnoPosition::THIGH_L)]     = new BNO_085(EnumBnoPosition::THIGH_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (EnumBnoPosition::TIBIA_L)]    = new BNO_085(EnumBnoPosition::TIBIA_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
    BNOs[static_cast<int> (EnumBnoPosition::THIGH_R)]     = new BNO_085(EnumBnoPosition::THIGH_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (EnumBnoPosition::TIBIA_R)]    = new BNO_085(EnumBnoPosition::TIBIA_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
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

    dataCore.setBnoAngles(EnumBnoAngle::THIGH_R, getValAngle(EnumBnoAngle::THIGH_R));
    dataCore.setBnoAngles(EnumBnoAngle::THIGH_L, getValAngle(EnumBnoAngle::THIGH_L));
    dataCore.setBnoAngles(EnumBnoAngle::TIBIA_R, getValAngle(EnumBnoAngle::TIBIA_R));
    dataCore.setBnoAngles(EnumBnoAngle::TIBIA_L, getValAngle(EnumBnoAngle::TIBIA_L));
    dataCore.setBnoAngles(EnumBnoAngle::EXO_BACK, getValAngle(EnumBnoAngle::EXO_BACK));
    dataCore.setBnoAngles(EnumBnoAngle::HIP_R, getValAngle(EnumBnoAngle::HIP_R));
    dataCore.setBnoAngles(EnumBnoAngle::HIP_L, getValAngle(EnumBnoAngle::HIP_L));
    dataCore.setBnoAngles(EnumBnoAngle::KNEE_R, getValAngle(EnumBnoAngle::KNEE_R));
    dataCore.setBnoAngles(EnumBnoAngle::KNEE_L, getValAngle(EnumBnoAngle::KNEE_L));
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
void BnoHandler::printBNOsStatus(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        Serial.print("\tIMU "); BNOs[i]->printName(); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(BNOs[i]->checkIfConnected());
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<EnumBnoAngle>(i)));
    }
}

void BnoHandler::printBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        Serial.print("\tIMU "); printName(EnumBnoAngle(i)); Serial.print("\t");
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<EnumBnoAngle>(i)));
    }
}

void BnoHandler::computeAngles() {
    float thighL = BNOs[static_cast<int> (EnumBnoPosition::THIGH_L)]->getEuler()[1];
    float tibiaL = -BNOs[static_cast<int> (EnumBnoPosition::TIBIA_L)]->getEuler()[1];
    float thighR = -BNOs[static_cast<int> (EnumBnoPosition::THIGH_R)]->getEuler()[1];
    float tibiaR = BNOs[static_cast<int> (EnumBnoPosition::TIBIA_R)]->getEuler()[1];
    float back = BNOs[static_cast<int> (EnumBnoPosition::EXO_BACK)]->getEuler()[1];

    // Compute Joint angles
    angles[static_cast<int> (EnumBnoAngle::HIP_L)] = abs(-thighL-back);
    angles[static_cast<int> (EnumBnoAngle::KNEE_L)] = abs(tibiaL-thighL);

    angles[static_cast<int> (EnumBnoAngle::HIP_R)] = abs(-thighR-back);
    angles[static_cast<int> (EnumBnoAngle::KNEE_R)] = abs(tibiaR-thighR);

    // Get angles "right away" to be used in calculations
    angles[static_cast<int> (EnumBnoAngle::EXO_BACK)] = back;
    angles[static_cast<int> (EnumBnoAngle::THIGH_L)] = thighL;
    angles[static_cast<int> (EnumBnoAngle::TIBIA_L)] = tibiaL;
    angles[static_cast<int> (EnumBnoAngle::THIGH_R)] = thighR;
    angles[static_cast<int> (EnumBnoAngle::TIBIA_R)] = tibiaR;
}

// Returns Yaw, same as previous implementation (BNO_055 used on Darianne)
float BnoHandler::getValAngle(EnumBnoAngle position){
    int pos = static_cast<int> (position);
    return this->angles[pos];
}

BNOStruct BnoHandler::getBNOData(EnumBnoPosition position){
    return BNOs[static_cast<int> (position)]->getData();
}

//print name of angle
void BnoHandler::printName(EnumBnoAngle position){

    switch (position)
    {
    case EnumBnoAngle::THIGH_L:
        Serial.print("THIGH_L");
        break;
    case EnumBnoAngle::THIGH_R:
        Serial.print("THIGH_R");
        break;
    case EnumBnoAngle::TIBIA_L:
        Serial.print("TIBIA_L");
        break;
    case EnumBnoAngle::TIBIA_R:
        Serial.print("TIBIA_R");
        break;
    case EnumBnoAngle::EXO_BACK:
        Serial.print("EXO_BACK");
        break;
    case EnumBnoAngle::HIP_L:
        Serial.print("HIP_L");
        break;
    case EnumBnoAngle::HIP_R:
        Serial.print("HIP_R");
        break;
    case EnumBnoAngle::KNEE_L:
        Serial.print("KNEE_L");
        break;
    case EnumBnoAngle::KNEE_R:
        Serial.print("KNEE_R");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
}