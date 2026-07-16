#include "bnoHandler.hpp"
#include "Arduino.h"
#include <Wire.h>
#include <cmath>
#include <cstring>

BnoHandler::BnoHandler(){
    this->mux = Multiplex();

    // Keep this order, position in arrays is same as EnumBnoPosition value
    muxChannels[exo_config::bnos::LEFT_THIGH] = exo_config::path::LEFT_MOUSTACHE_CHANNEL;
    muxChannels[exo_config::bnos::RIGHT_THIGH] = exo_config::path::RIGHT_MOUSTACHE_CHANNEL;
    muxChannels[exo_config::bnos::LEFT_SHIN] = exo_config::path::LEFT_MOUSTACHE_CHANNEL;
    muxChannels[exo_config::bnos::RIGHT_SHIN] = exo_config::path::RIGHT_MOUSTACHE_CHANNEL;
    muxChannels[exo_config::bnos::EXO_BACK] = exo_config::path::BACK_CHANNEL;
    muxChannels[exo_config::bnos::MOBO] = exo_config::path::MOBO_CHANNEL;

    i2cAddresses[exo_config::bnos::LEFT_THIGH] = exo_config::path::ADDRESS_1;
    i2cAddresses[exo_config::bnos::RIGHT_THIGH] = exo_config::path::ADDRESS_1;
    i2cAddresses[exo_config::bnos::LEFT_SHIN] = exo_config::path::ADDRESS_2;
    i2cAddresses[exo_config::bnos::RIGHT_SHIN] = exo_config::path::ADDRESS_2;
    i2cAddresses[exo_config::bnos::EXO_BACK] = exo_config::path::ADDRESS_1;
    i2cAddresses[exo_config::bnos::MOBO] = exo_config::path::ADDRESS_2;

    bufferIndexLeft = 0;
    bufferIndexRight = 0;

    for(int i = 0; i < exo_config::bnos::BUFFER_SIZE; i++)
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

    if (exo_config::debug::BNO) Serial.print("===== BnoHandler STARTING =====\n");

    for (int i = 0; i < bnoDevices.size(); i++){
        mux.selectChannel(muxChannels[i]);

        const bool isConnected = bnoDevices[i].begin(i2cAddresses[i], Wire);
        setupReports(i);
        bnoConnected[i] = isConnected;

        if (isConnected){
            connected++;
        }

        if (exo_config::debug::BNO) {
            Serial.print("BNO "); Serial.print(i); Serial.print("\t");
            Serial.print("Connected: "); Serial.println(isConnected);
            delay(500);
        }
    }

    // At least one BNO is up and running
    return connected > 0;
}

void BnoHandler::setupReports(uint8_t position)
{
    const size_t index = bnoIndex(position);
    // Enable common reports using SparkFun API. Time between reports set to 10 (library-specific unit).
    bnoDevices[index].enableGravity(10);
    bnoDevices[index].enableLinearAccelerometer(10);

}

bool BnoHandler::checkIfConnected(uint8_t position)
{
    const size_t index = bnoIndex(position);
    mux.selectChannel(muxChannels[index]);
    Wire.beginTransmission(i2cAddresses[index]);
    const bool connected = (Wire.endTransmission() == 0);
    bnoConnected[index] = connected;
    return connected;
}

void BnoHandler::requestData(){
    for (int i = 0; i < exo_config::bnos::AMOUNT; i++){
        if (!bnoConnected[i]) continue;
        mux.selectChannel(muxChannels[i]);

        if (bnoDevices[i].hasReset()) {
            setupReports(i);
        }

        // Read available reports and populate compatibility structures
        while (bnoDevices[i].dataAvailable()) {

            float gX = bnoDevices[i].getGravityX();
            float gY = bnoDevices[i].getGravityY();
            float gZ = bnoDevices[i].getGravityZ();

            if (exo_config::bnos::MOBO == i)
                BNOAngles[i] = -1*(degrees(atan2(gY, gX)) + 180);
            else
                BNOAngles[i] = degrees(atan2(gX, gY)) + 180;

            if (BNOAngles[i] > 180) BNOAngles[i] -= 360;
            else if (BNOAngles[i] <= -180) BNOAngles[i] += 360;

            if (exo_config::bnos::LEFT_THIGH == i
                || exo_config::bnos::LEFT_SHIN == i)
            {
                BNOAngles[i] = -1*BNOAngles[i];
            }
        
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

void BnoHandler::getAngle(float angles[exo_config::bnos::AMOUNT])
{
    memcpy(angles, &BNOAngles, sizeof(BNOAngles));
}

void BnoHandler::updateBuffer(uint8_t position)
{
    const int16_t yAccel = getLinAccelYScaled(position);
    if (position==exo_config::bnos::LEFT_SHIN){
        linAccelBufferLeft[bufferIndexLeft] = abs(yAccel) < (exo_config::bnos::ACCEL_BUFFER_THRESHOLD + offset);
        if(bufferIndexLeft < (exo_config::bnos::BUFFER_SIZE-1)) bufferIndexLeft++;
        else bufferIndexLeft = 0;
    }
    else if (position==exo_config::bnos::RIGHT_SHIN){
        linAccelBufferRight[bufferIndexRight] = abs(yAccel) < (exo_config::bnos::ACCEL_BUFFER_THRESHOLD + offset);
        if(bufferIndexRight < (exo_config::bnos::BUFFER_SIZE-1)) bufferIndexRight++;
        else bufferIndexRight = 0;
    }
}

void BnoHandler::getGroundedState(bool grounded[exo_config::bnos::NB_LEG])
{
    float bufferAvg = 0;
    updateBuffer(exo_config::bnos::LEFT_SHIN);
    for(int i=0; i<exo_config::bnos::BUFFER_SIZE;i++)
    {
        bufferAvg += linAccelBufferLeft[i];
    }
    bufferAvg /= exo_config::bnos::BUFFER_SIZE;
    grounded[exo_config::bnos::LEFT_LEG] = bufferAvg >= exo_config::bnos::ACCEL_BUFFER_THRESHOLD;

    bufferAvg = 0;
    updateBuffer(exo_config::bnos::RIGHT_SHIN);
    for(int i=0; i<exo_config::bnos::BUFFER_SIZE;i++)
    {
        bufferAvg += linAccelBufferRight[i];
    }
    bufferAvg /= exo_config::bnos::BUFFER_SIZE;
    grounded[exo_config::bnos::RIGHT_LEG] = bufferAvg >= exo_config::bnos::ACCEL_BUFFER_THRESHOLD;
}

int16_t BnoHandler::getLinAccelYScaled(uint8_t position)
{
    const float y = linearAccelerations[bnoIndex(position)].y;
    return static_cast<int16_t>(y * 256.0f);
}

void BnoHandler::printName(uint8_t position){

    switch (position)
    {
    case exo_config::bnos::LEFT_THIGH:
        Serial.print("THIGH_L");
        break;
    case exo_config::bnos::RIGHT_THIGH:
        Serial.print("THIGH_R");
        break;
    case exo_config::bnos::LEFT_SHIN:
        Serial.print("TIBIA_L");
        break;
    case exo_config::bnos::RIGHT_SHIN:
        Serial.print("TIBIA_R");
        break;
    case exo_config::bnos::EXO_BACK:
        Serial.print("EXO_BACK");
        break;
    case exo_config::bnos::MOBO:
        Serial.print("MOBO");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
}

void BnoHandler::printBNOData(uint8_t position){

    Serial.print("IMU "); printName(position); Serial.print("\n");
    Serial.print("Y: "); Serial.print(BNOAngles[bnoIndex(position)]);
    Serial.print("\n");
}

// Print relevant IMU information
void BnoHandler::printBNOsStatus(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        Serial.print("\tIMU "); printName(i); Serial.print("\t");
        Serial.print("LINK: "); Serial.print(checkIfConnected(i));
    }
}

void BnoHandler::printBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i<=endIndex; i++){
        printBNOData(i);
    }
}

void BnoHandler::printConnectedBNOsData(int startIndex, int endIndex){
    for (int i = startIndex; i <= endIndex; i++){
        if (bnoConnected[i]) {
            printBNOData(i);
        }
    }
}