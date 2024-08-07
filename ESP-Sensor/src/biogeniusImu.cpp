#include "biogeniusImu.h"
#include "Arduino.h"
using namespace std;

IMU::IMU(){
    this->mux = Multiplex();
    // Hips will use 0x4A, Knee will use 0x4B
    // Keep this order, position in vector is same as enum value
    BNOs[static_cast<int> (enumIMU::HIP_L)]     = new BNO(enumIMU::HIP_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_L)]    = new BNO(enumIMU::KNEE_L, LEFT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
    BNOs[static_cast<int> (enumIMU::HIP_R)]     = new BNO(enumIMU::HIP_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4A);
    BNOs[static_cast<int> (enumIMU::KNEE_R)]    = new BNO(enumIMU::KNEE_R, RIGHT_MOUSTACHE_MUX_CHANNEL, &this->mux, 0x4B);
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
    for (int i = startIndex; i<endIndex; i++){
        Serial.print("\tIMU "); BNOs[i]->printName(); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(BNOs[i]->checkIfConnected());
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<enumIMU>(i)));
    }
}

void IMU::computeAngles() {

    // Angles on Y axis (getEuler()[1])
    //LE - fait du sens en Y (car on inverse la central inertiel)
    //En Z, on a pas besoin de le changer (hypothèse de Édouard)
    //En X, je ne sais pas encore cv être a vérifier

    //VÉRIFIER SI IL Y A PAS UN FOR LOOP QUI SCRAPERAIT TOUTES LES AFFAIRES 
    //-----------Y axis----------//
    float thighL = BNOs[static_cast<int> (enumIMU::HIP_L)]->getEuler()[1];
    float tibiaL = -BNOs[static_cast<int> (enumIMU::KNEE_L)]->getEuler()[1];
    float thighR = -BNOs[static_cast<int> (enumIMU::HIP_R)]->getEuler()[1];
    float tibiaR = BNOs[static_cast<int> (enumIMU::KNEE_R)]->getEuler()[1];
    float back = BNOs[static_cast<int> (enumIMU::EXO_BACK)]->getEuler()[1];

    //-----------X axis----------//
    float thighLX = BNOs[static_cast<int> (enumIMU::HIP_L)]->getEuler()[0];
    float tibiaLX = BNOs[static_cast<int> (enumIMU::KNEE_L)]->getEuler()[0];
    float thighRX = BNOs[static_cast<int> (enumIMU::HIP_R)]->getEuler()[0];
    float tibiaRX = BNOs[static_cast<int> (enumIMU::KNEE_R)]->getEuler()[0];
    float backX   = BNOs[static_cast<int> (enumIMU::EXO_BACK)]->getEuler()[0];
    
    //-----------Z axis----------//
    float thighLZ = BNOs[static_cast<int> (enumIMU::HIP_L)]->getEuler()[2];
    float tibiaLZ = BNOs[static_cast<int> (enumIMU::KNEE_L)]->getEuler()[2];
    float thighRZ = BNOs[static_cast<int> (enumIMU::HIP_R)]->getEuler()[2];
    float tibiaRZ = BNOs[static_cast<int> (enumIMU::KNEE_R)]->getEuler()[2];
    float backZ   = BNOs[static_cast<int> (enumIMU::EXO_BACK)]->getEuler()[2];

    //-----Y relative angle------//
    angles[static_cast<int> (enumIMU::HIP_L)] = abs(-thighL-back);
    angles[static_cast<int> (enumIMU::KNEE_L)] = abs(tibiaL-thighL);
    angles[static_cast<int> (enumIMU::HIP_R)] = abs(-thighR-back);
    angles[static_cast<int> (enumIMU::KNEE_R)] = abs(tibiaR-thighR);

    angles[static_cast<int> (enumIMU::EXO_BACK)] = back;
    angles[static_cast<int> (enumIMU::TIBIA_L)] = tibiaL;
    angles[static_cast<int> (enumIMU::TIBIA_R)] = tibiaR;
    angles[static_cast<int> (enumIMU::THIGH_L)] = thighL;
    angles[static_cast<int> (enumIMU::THIGH_R)] = thighR;

    //-----X relative angle------//
    ////--- Pas besoin de cette cochonerie là ---////
    angles[static_cast<int> (enumIMU::HIP_LX)] = abs(thighLX - backX);//HIP_LX
    angles[static_cast<int> (enumIMU::KNEE_LX)] = abs(tibiaLX - thighLX);//KNEE_LX
    angles[static_cast<int> (enumIMU::HIP_RX)] = abs(thighRX - backX);//HIP_RX
    angles[static_cast<int> (enumIMU::KNEE_RX)] = abs(tibiaRX - thighRX);//KNEE_RX
    ////-----------------------------------------////
    
    angles[static_cast<int> (enumIMU::EXO_BACKX)] = backX;//EXO_BACKX
    angles[static_cast<int> (enumIMU::TIBIA_LX)] = tibiaLX;//TIBIA_LX
    angles[static_cast<int> (enumIMU::TIBIA_RX)] = tibiaRX;//TIBIA_RX
    angles[static_cast<int> (enumIMU::THIGH_LX)] = thighLX;//THIGH_LX
    angles[static_cast<int> (enumIMU::THIGH_RX)] = thighRX;//THIGH_RX

    //-----Z relative angle------//
    ////--- Pas besoin de cette cochonerie là ---////
    angles[static_cast<int> (enumIMU::HIP_LZ)] = abs(thighLZ - backZ);//HIP_LZ
    angles[static_cast<int> (enumIMU::KNEE_LZ)] = abs(tibiaLZ - thighLZ);//KNEE_LZ
    angles[static_cast<int> (enumIMU::HIP_RZ)] = abs(thighRZ - backZ);//HIP_RZ
    angles[static_cast<int> (enumIMU::KNEE_RZ)] = abs(tibiaRZ - thighRZ);//KNEE_RZ
    ////-----------------------------------------////

    angles[static_cast<int> (enumIMU::EXO_BACKZ)] = backZ;//EXO_BACKZ
    angles[static_cast<int> (enumIMU::TIBIA_LZ)] = tibiaLZ;//TIBIA_LZ
    angles[static_cast<int> (enumIMU::TIBIA_RZ)] = tibiaRZ;//TIBIA_RZ
    angles[static_cast<int> (enumIMU::THIGH_LZ)] = thighLZ;//THIGH_LZ
    angles[static_cast<int> (enumIMU::THIGH_RZ)] = thighRZ;//THIGH_RZ

}

// Returns Yaw, same as previous implementation
float IMU::getValAngle(enumIMU position){
    int pos = static_cast<int> (position);
    return this->angles[pos];
}

BNOStruct IMU::getBNOData(enumIMU position){
    return BNOs[static_cast<int> (position)]->getData();
}
