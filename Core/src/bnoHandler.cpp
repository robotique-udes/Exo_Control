#include "bnoHandler.h"
#include "Arduino.h"
#include "define.h"
#include <Wire.h>
#include <cstring>
using namespace std;

BnoHandler::BnoHandler(){
    this->mux = Multiplex();

    // Keep this order, position in arrays is same as EnumBnoPosition value
    muxChannels[bnoIndex(EnumBnoPosition::THIGH_L)] = LEFT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[bnoIndex(EnumBnoPosition::THIGH_R)] = RIGHT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[bnoIndex(EnumBnoPosition::TIBIA_L)] = LEFT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[bnoIndex(EnumBnoPosition::TIBIA_R)] = RIGHT_MOUSTACHE_MUX_CHANNEL;
    muxChannels[bnoIndex(EnumBnoPosition::EXO_BACK)] = BACK_MUX_CHANNEL;
    muxChannels[bnoIndex(EnumBnoPosition::MOBO)] = MOBO_MUX_CHANNEL;

    i2cAddresses[bnoIndex(EnumBnoPosition::THIGH_L)] = BNO_ADDRESS_1;
    i2cAddresses[bnoIndex(EnumBnoPosition::THIGH_R)] = BNO_ADDRESS_1;
    i2cAddresses[bnoIndex(EnumBnoPosition::TIBIA_L)] = BNO_ADDRESS_2;
    i2cAddresses[bnoIndex(EnumBnoPosition::TIBIA_R)] = BNO_ADDRESS_2;
    i2cAddresses[bnoIndex(EnumBnoPosition::EXO_BACK)] = BNO_ADDRESS_1;
    i2cAddresses[bnoIndex(EnumBnoPosition::MOBO)] = BNO_ADDRESS_2;

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
        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        mux.selectChannel(muxChannels[i]);

        const bool isConnected = bnoDevices[i].begin_I2C(i2cAddresses[i], &Wire) && setupReports(position);
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

    dataCore.setBnoAngle(EnumBnoPosition::THIGH_R, getValAngle(EnumBnoPosition::THIGH_R));
    dataCore.setBnoAngle(EnumBnoPosition::THIGH_L, getValAngle(EnumBnoPosition::THIGH_L));
    dataCore.setBnoAngle(EnumBnoPosition::TIBIA_R, getValAngle(EnumBnoPosition::TIBIA_R));
    dataCore.setBnoAngle(EnumBnoPosition::TIBIA_L, getValAngle(EnumBnoPosition::TIBIA_L));
    dataCore.setBnoAngle(EnumBnoPosition::EXO_BACK, getValAngle(EnumBnoPosition::EXO_BACK));
    

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

        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        mux.selectChannel(muxChannels[i]);

        if (bnoDevices[i].wasReset()) {
            setupReports(position);
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
        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        Serial.print("\tIMU "); printName(position); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(checkIfConnected(position));
        Serial.print("\tCOMPUTE ANGLE:\t"); Serial.println(this->getValAngle(position));
    }
}

void BnoHandler::printBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        printBNOData(static_cast<EnumBnoPosition>(i));
    }
}

void BnoHandler::printConnectedBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i <= endIndex; i++){
        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        if (bnoConnected[i]) {
            printBNOData(position);
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

    // Get angles "right away" to be used in calculations
    angles[bnoIndex(EnumBnoPosition::EXO_BACK)] = back;
    angles[bnoIndex(EnumBnoPosition::THIGH_L)] = thighL;
    angles[bnoIndex(EnumBnoPosition::TIBIA_L)] = tibiaL;
    angles[bnoIndex(EnumBnoPosition::THIGH_R)] = thighR;
    angles[bnoIndex(EnumBnoPosition::TIBIA_R)] = tibiaR;
}

// Returns Yaw, same as previous implementation (BNO_055 used on Darianne)
float BnoHandler::getValAngle(EnumBnoPosition position){
    return this->angles[bnoIndex(position)];
}

sh2_SensorValue_t BnoHandler::getBNOData(EnumBnoPosition position){
    return bnoRotation[bnoIndex(position)];
}

sh2_SensorValue_t* BnoHandler::getBNODataPointer(EnumBnoPosition position){
    return &bnoRotation[bnoIndex(position)];
}

//print name of angle
void BnoHandler::printName(EnumBnoPosition position){

    switch (position)
    {
    case EnumBnoPosition::THIGH_L:
        Serial.print("THIGH_L");
        break;
    case EnumBnoPosition::THIGH_R:
        Serial.print("THIGH_R");
        break;
    case EnumBnoPosition::TIBIA_L:
        Serial.print("TIBIA_L");
        break;
    case EnumBnoPosition::TIBIA_R:
        Serial.print("TIBIA_R");
        break;
    case EnumBnoPosition::EXO_BACK:
        Serial.print("EXO_BACK");
        break;
    case EnumBnoPosition::MOBO:
        Serial.print("MOBO");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
}

void BnoHandler::printBNOData(EnumBnoPosition position){
    const sh2_RotationVectorWAcc_t &q = bnoRotation[bnoIndex(position)].un.rotationVector;
    const float w = q.real;
    const float x = q.i;
    const float y = q.j;
    const float z = q.k;

    const float sinr_cosp = 2.0f * (w * x + y * z);
    const float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    const float rollX = atan2(sinr_cosp, cosr_cosp) * RAD_TO_DEG;

    float sinp = 2.0f * (w * y - z * x);
    if (sinp > 1.0f) sinp = 1.0f;
    if (sinp < -1.0f) sinp = -1.0f;
    const float pitchY = asin(sinp) * RAD_TO_DEG;

    const float siny_cosp = 2.0f * (w * z + x * y);
    const float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    const float yawZ = atan2(siny_cosp, cosy_cosp) * RAD_TO_DEG;

    Serial.print("IMU "); printName(position); Serial.print("\n");
    Serial.print("X: "); Serial.print(rollX);
    Serial.print("\tY: "); Serial.print(pitchY);
    Serial.print("\tZ: "); Serial.print(yawZ);
    Serial.print("\n");
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
    const size_t index = bnoIndex(position);
    memset(&bnoRotation[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoAccel[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoLinAccel[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoGyro[index], 0, sizeof(sh2_SensorValue_t));
    memset(&bnoMag[index], 0, sizeof(sh2_SensorValue_t));
}

bool BnoHandler::setupReports(EnumBnoPosition position)
{
    const size_t index = bnoIndex(position);
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

bool BnoHandler::checkIfConnected(EnumBnoPosition position)
{
    const size_t index = bnoIndex(position);
    mux.selectChannel(muxChannels[index]);
    Wire.beginTransmission(i2cAddresses[index]);
    const bool connected = (Wire.endTransmission() == 0);
    bnoConnected[index] = connected;
    return connected;
}

float BnoHandler::getPitchDegrees(EnumBnoPosition position)
{
    const sh2_RotationVectorWAcc_t &q = bnoRotation[bnoIndex(position)].un.rotationVector;
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
    const float y = bnoLinAccel[bnoIndex(position)].un.linearAcceleration.y;
    return static_cast<int16_t>(y * 256.0f);
}