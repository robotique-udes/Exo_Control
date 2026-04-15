#include "bnoHandler.h"
#include "Arduino.h"
#include "define.h"
#include <Wire.h>
#include <cstring>
using namespace std;

BnoHandler::BnoHandler(){
    this->mux = Multiplex();

    // Keep this order, position in arrays is same as EnumBnoPosition value
    muxChannels[static_cast<int>(EnumBnoPosition::THIGH_L)] = LEFT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[static_cast<int>(EnumBnoPosition::THIGH_R)] = RIGHT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[static_cast<int>(EnumBnoPosition::TIBIA_L)] = LEFT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[static_cast<int>(EnumBnoPosition::TIBIA_R)] = RIGHT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[static_cast<int>(EnumBnoPosition::EXO_BACK)] = 0;
    muxChannels[static_cast<int>(EnumBnoPosition::MOBO)] = 3;

    i2cAddresses[static_cast<int>(EnumBnoPosition::THIGH_L)] = 0x4A;
    i2cAddresses[static_cast<int>(EnumBnoPosition::THIGH_R)] = 0x4A;
    i2cAddresses[static_cast<int>(EnumBnoPosition::TIBIA_L)] = 0x4B;
    i2cAddresses[static_cast<int>(EnumBnoPosition::TIBIA_R)] = 0x4B;
    i2cAddresses[static_cast<int>(EnumBnoPosition::EXO_BACK)] = 0x4A;
    i2cAddresses[static_cast<int>(EnumBnoPosition::MOBO)] = 0x4B;

    bufferIndexLeft = 0;
    bufferIndexRight = 0;

    for(int i = 0; i < BUFFER_SIZE; i++)
    {
        linAccelBufferLeft[i]=0;
        linAccelBufferRight[i]=0;
    }

    for (int i = 0; i < bnoDevices.size(); i++) {
        bnoConnected[i] = false;
        resetData(static_cast<EnumBnoPosition>(i));
    }

    // DataCore currently stores 5 BNO pointers (no MOBO slot)
    for (int i = 0; i < 5 && i < bnoDevices.size(); i++){
        dataCore.setBnoStruct(static_cast<EnumBnoPosition>(i), &bnoRotation[i]);
    }
}

// OPTIONAL: Check if a BNO is connected
bool BnoHandler::begin(){
    int8_t connected = 0;

    if (IMU_DEBUG) Serial.print("===== BnoHandler STARTING =====\n");

    for (int i = 0; i < bnoDevices.size(); i++){
        mux.selectChannel(muxChannels[i]);

        const bool isConnected = bnoDevices[i].begin_I2C(i2cAddresses[i], &Wire) && setupReports(i);
        bnoConnected[i] = isConnected;

        if (isConnected){
            connected++;
        }

        if (IMU_DEBUG) {
            Serial.print("BNO "); Serial.print(i); Serial.print("\t");
            Serial.print("Connected: "); Serial.println(isConnected);
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
    for (int i = 0; i < bnoDevices.size(); i++){
        if (!bnoConnected[i]) {
            continue;
        }

        mux.selectChannel(muxChannels[i]);

        if (bnoDevices[i].wasReset()) {
            setupReports(i);
        }

        while (bnoDevices[i].getSensorEvent(&sensorValue)) {

            switch (sensorValue.sensorId) {
                case SH2_ROTATION_VECTOR:
                    bnoRotation[i] = sensorValue;
                    break;

                case SH2_LINEAR_ACCELERATION:
                    bnoLinAccel[i] = sensorValue;
                    break;

                case SH2_ACCELEROMETER:
                    bnoAccel[i] = sensorValue;
                    break;

                case SH2_GYROSCOPE_CALIBRATED:
                    bnoGyro[i] = sensorValue;
                    break;

                case SH2_MAGNETIC_FIELD_CALIBRATED:
                    bnoMag[i] = sensorValue;
                    break;

                default:
                    break;
            }
        }
    }

    this->computeAngles();

    last_update = millis();
}

// Print relevant IMU information
void BnoHandler::printBNOsStatus(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        Serial.print("\tIMU "); printName(static_cast<EnumBnoAngle>(i)); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(checkIfConnected(i));
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(static_cast<EnumBnoAngle>(i)));
    }
}

void BnoHandler::printBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        printBNOData(static_cast<EnumBnoPosition>(i));
    }
}

void BnoHandler::printConnectedBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i <= endIndex; i++){
        if (checkIfConnected(i)) {
            printBNOData(static_cast<EnumBnoPosition>(i));
        }
    }
}

void BnoHandler::updateBuffer(EnumBnoPosition position)
{
    const int16_t yAccel = getLinAccelYScaled(position);
    if (position==EnumBnoPosition::TIBIA_L){
        linAccelBufferLeft[bufferIndexLeft] = abs(yAccel) < (ACCEL_THRESHOLD + offset);
        if(bufferIndexLeft < (BUFFER_SIZE-1)) bufferIndexLeft++;
        else bufferIndexLeft = 0;
    }
    else if (position==EnumBnoPosition::TIBIA_R){
        linAccelBufferRight[bufferIndexRight] = abs(yAccel) < (ACCEL_THRESHOLD + offset);
        if(bufferIndexRight < (BUFFER_SIZE-1)) bufferIndexRight++;
        else bufferIndexRight = 0;
    }
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
    Serial.print(" Buffer avg: ");
    Serial.print(bufferAvg);
    return bufferAvg >= ACCEL_BUFFER_THRESHOLD;
}

void BnoHandler::computeAngles() {
    float thighL = getPitchDegrees(EnumBnoPosition::THIGH_L);
    float tibiaL = -getPitchDegrees(EnumBnoPosition::TIBIA_L);
    float thighR = -getPitchDegrees(EnumBnoPosition::THIGH_R);
    float tibiaR = getPitchDegrees(EnumBnoPosition::TIBIA_R);
    float back = getPitchDegrees(EnumBnoPosition::EXO_BACK);

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

sh2_SensorValue_t BnoHandler::getBNOData(EnumBnoPosition position){
    return bnoRotation[static_cast<int> (position)];
}

sh2_SensorValue_t* BnoHandler::getBNODataPointer(EnumBnoPosition position){
    return &bnoRotation[static_cast<int> (position)];
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
    const int index = static_cast<int>(position);
    Serial.print("IMU "); printName(static_cast<EnumBnoAngle>(position)); Serial.print("\n");
    Serial.print("Accel: ");
    Serial.print(bnoAccel[index].un.accelerometer.x); Serial.print("\t");
    Serial.print(bnoAccel[index].un.accelerometer.y); Serial.print("\t");
    Serial.print(bnoAccel[index].un.accelerometer.z); Serial.print("\t");
    
    Serial.print("L-Acc: "); 
    Serial.print(bnoLinAccel[index].un.linearAcceleration.x); Serial.print("\t");
    Serial.print(bnoLinAccel[index].un.linearAcceleration.y); Serial.print("\t");
    Serial.print(bnoLinAccel[index].un.linearAcceleration.z); Serial.print("\n");
}

void BnoHandler::printGroundState()
{
    Serial.print(" Left ground state: \t");
    Serial.print(dataCore.getLeftGrounded());
    Serial.print(" Right ground state: \t");
    Serial.print(dataCore.getRightGrounded());
    Serial.print(" Threshold: \t");
    Serial.print(ACCEL_THRESHOLD + offset);

}

void BnoHandler::resetData(EnumBnoPosition position)
{
    const int index = static_cast<int>(position);
    memset(&bnoRotation[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoAccel[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoLinAccel[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoGyro[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoMag[index], 0, sizeof(sh2_SensorValue_t));
}

bool BnoHandler::setupReports(int index)
{
    const bool ok =
        bnoDevices[index].enableReport(SH2_ACCELEROMETER) &&
        bnoDevices[index].enableReport(SH2_GYROSCOPE_CALIBRATED) &&
        bnoDevices[index].enableReport(SH2_MAGNETIC_FIELD_CALIBRATED) &&
        bnoDevices[index].enableReport(SH2_LINEAR_ACCELERATION) &&
        bnoDevices[index].enableReport(SH2_ROTATION_VECTOR);

    if (!ok) {
        if (IMU_DEBUG) {
            Serial.print("Failed to configure BNO reports for index ");
            Serial.println(index);
        }
        bnoConnected[index] = false;
    }

    return ok;
}

bool BnoHandler::checkIfConnected(int index)
{
    mux.selectChannel(muxChannels[index]);
    Wire.beginTransmission(i2cAddresses[index]);
    const bool connected = (Wire.endTransmission() == 0);
    bnoConnected[index] = connected;
    return connected;
}

float BnoHandler::getPitchDegrees(EnumBnoPosition position)
{
    const sh2_RotationVectorWAcc_t &q = bnoRotation[static_cast<int>(position)].un.rotationVector;
    const float w = q.real;
    const float x = q.i;
    const float y = q.j;
    const float z = q.k;

    const float sqw = sq(w);
    const float sqx = sq(x);
    const float sqy = sq(y);
    const float sqz = sq(z);

    const float pitch = asin(-2.0f * (x * z - y * w) / (sqx + sqy + sqz + sqw));
    return pitch * RAD_TO_DEG;
}

int16_t BnoHandler::getLinAccelYScaled(EnumBnoPosition position)
{
    const float y = bnoLinAccel[static_cast<int>(position)].un.linearAcceleration.y;
    return static_cast<int16_t>(y * 256.0f);
}