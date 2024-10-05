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
    bufferIndexLeft = 0;
    bufferIndexRight = 0;
    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        linAccelBufferLeft[i]=0;
        linAccelBufferRight[i]=0;
    }

    for (int i = 0; i < BNOs.size(); i++){
        dataCore.setBnoStruct(static_cast<EnumBnoPosition>(i), BNOs[i]->getDataPointer());
    }
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

    //Ground status
    dataCore.setRightGrounded(getLinAccel(EnumBnoPosition::TIBIA_R));
    dataCore.setLeftGrounded(getLinAccel(EnumBnoPosition::TIBIA_L));
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
        /**
        Serial.print("\tIMU "); printName(EnumBnoAngle(i)); Serial.print("\t");
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<EnumBnoAngle>(i)));
        */
        printBNOData(static_cast<EnumBnoPosition>(i));
    }
}

void BnoHandler::updateBuffer(EnumBnoPosition position)
{
    BNOStruct data = BNOs[static_cast<int> (position)]->getData();
    if (position==EnumBnoPosition::TIBIA_L){
        linAccelBufferLeft[bufferIndexLeft] = data.lin_acceleration[1];
        if(bufferIndexLeft < (BUFFER_SIZE-1)) bufferIndexLeft++;
        else bufferIndexLeft = 0;
    }
    else if (position==EnumBnoPosition::TIBIA_R){
        linAccelBufferRight[bufferIndexRight] = data.lin_acceleration[1];
        if(bufferIndexRight < (BUFFER_SIZE-1)) bufferIndexRight++;
        else bufferIndexRight = 0;
    }
    //Serial.print(data.lin_acceleration[1]);Serial.print("  \t");
}

bool BnoHandler::getLinAccel(EnumBnoPosition position)
{
    float bufferAvg = 0;
    updateBuffer(position);
    for(int i=0; i<BUFFER_SIZE;i++)
    {
        if (position==EnumBnoPosition::TIBIA_L) bufferAvg += linAccelBufferLeft[i];
        else if (position==EnumBnoPosition::TIBIA_R) bufferAvg += linAccelBufferRight[i];
    }
    bufferAvg /= BUFFER_SIZE;
    return bufferAvg < (ACCEL_THRESHOLD + offset);
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

BNOStruct* BnoHandler::getBNODataPointer(EnumBnoPosition position){
    return BNOs[static_cast<int> (position)]->getDataPointer();
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

void BnoHandler::printBNOData(EnumBnoPosition position){
    // Lets print lin_acceleration and acceleration pretty
    BNOStruct data = BNOs[static_cast<int> (position)]->getData();
    Serial.print("IMU "); printName(static_cast<EnumBnoAngle>(position)); Serial.print("\n");
    Serial.print("Accel: ");
    Serial.print(data.acceleration[0]); Serial.print("\t");
    Serial.print(data.acceleration[1]); Serial.print("\t");
    Serial.print(data.acceleration[2]); Serial.print("\t");
    
    Serial.print("L-Acc: "); 
    Serial.print(data.lin_acceleration[0]); Serial.print("\t");
    Serial.print(data.lin_acceleration[1]); Serial.print("\t");
    Serial.print(data.lin_acceleration[2]); Serial.print("\n");
}

void BnoHandler::printGroundState()
{
    Serial.print(" Right ground state: \t");
    Serial.print(dataCore.getRightGrounded());
    Serial.print(" Left ground state: \t");
    Serial.print(dataCore.getLeftGrounded());
    Serial.print(" Threshold: \t");
    Serial.print(ACCEL_THRESHOLD + offset);

}