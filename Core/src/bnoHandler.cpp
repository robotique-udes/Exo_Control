#include "bnoHandler.h"
#include "Arduino.h"
#include "define.h"
#include <Wire.h>
#include <cmath>
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
        BNOAngles[i] = 0;
    }

}

bool BnoHandler::begin(){
    int8_t connected = 0;

    if (IMU_DEBUG) Serial.print("===== BnoHandler STARTING =====\n");

    for (int i = 0; i < bnoDevices.size(); i++){
        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        mux.selectChannel(muxChannels[i]);

        const bool isConnected = bnoDevices[i].begin(i2cAddresses[i], Wire);
        setupReports(position);
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

    // At least one BNO is up and running
    return connected > 0;
}

void BnoHandler::setupReports(EnumBnoPosition position)
{
    const size_t index = bnoIndex(position);
    // Enable common reports using SparkFun API. Time between reports set to 10 (library-specific unit).
    bnoDevices[index].enableGravity(10);
    bnoDevices[index].enableLinearAccelerometer(10);

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

void BnoHandler::requestData(){
    for (int i = 0; i < bnoDevices.size(); i++){
        if (!bnoConnected[i]) continue;

        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        mux.selectChannel(muxChannels[i]);

        if (bnoDevices[i].hasReset()) {
            setupReports(position);
        }

        // Read available reports and populate compatibility structures
        while (bnoDevices[i].dataAvailable()) {

            float gX = bnoDevices[i].getGravityX();
            float gY = bnoDevices[i].getGravityY();
            float gZ = bnoDevices[i].getGravityZ();
            BNOAngles[i] = degrees(atan2(gX, gY)) + 180;
            
            // Linear acceleration
            float lax, lay, laz; uint8_t lac;
            bnoDevices[i].getLinAccel(lax, lay, laz, lac);
            linearAccelerations[i].x = lax;
            linearAccelerations[i].y = lay;
            linearAccelerations[i].z = laz;

        }
    }

    last_update = millis();
}

angleOutput_t BnoHandler::getAngle()
{
    angleOutput_t angle;
    angle.hipLeft = BNOAngles[bnoIndex(EnumBnoPosition::THIGH_L)];
    angle.hipRight = BNOAngles[bnoIndex(EnumBnoPosition::THIGH_R)];
    angle.KneeLeft = BNOAngles[bnoIndex(EnumBnoPosition::TIBIA_L)];
    angle.KneeRight = BNOAngles[bnoIndex(EnumBnoPosition::TIBIA_R)];
    angle.back = BNOAngles[bnoIndex(EnumBnoPosition::MOBO)];

    return angle;
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

groundedOutput_t BnoHandler::getGroundedState()
{
    groundedOutput_t groundedOutput;

    float bufferAvg = 0;
    updateBuffer(EnumBnoPosition::TIBIA_L);
    for(int i=0; i<BUFFER_SIZE;i++)
    {
        bufferAvg += linAccelBufferLeft[i];
    }
    bufferAvg /= BUFFER_SIZE;

    groundedOutput.isLeftGrounded = bufferAvg >= ACCEL_BUFFER_THRESHOLD;

    bufferAvg = 0;
    updateBuffer(EnumBnoPosition::TIBIA_R);
    for(int i=0; i<BUFFER_SIZE;i++)
    {
        bufferAvg += linAccelBufferRight[i];
    }
    bufferAvg /= BUFFER_SIZE;

    groundedOutput.isRightGrounded = bufferAvg >= ACCEL_BUFFER_THRESHOLD;

    return groundedOutput;
}

int16_t BnoHandler::getLinAccelYScaled(EnumBnoPosition position)
{
    const float y = linearAccelerations[bnoIndex(position)].y;
    return static_cast<int16_t>(y * 256.0f);
}

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

    Serial.print("IMU "); printName(position); Serial.print("\n");
    Serial.print("Y: "); Serial.print(BNOAngles[bnoIndex(position)]);
    Serial.print("\n");
}

// Print relevant IMU information
void BnoHandler::printBNOsStatus(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        const EnumBnoPosition position = static_cast<EnumBnoPosition>(i);
        Serial.print("\tIMU "); printName(position); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(checkIfConnected(position));
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