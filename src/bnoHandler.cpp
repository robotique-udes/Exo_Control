#include "bnoHandler.h"
#include "Arduino.h"
#include <Wire.h>
#include <cmath>
#include <cstring>
using namespace std;

BnoHandler::BnoHandler(){
    this->mux = Multiplex();

    // Keep this order, position in arrays is same as EnumBnoPosition value
    muxChannels[bno_config::left_thigh] = path_config::left_moustache_channel;
    muxChannels[bno_config::right_thigh] = path_config::right_moustache_channel;
    muxChannels[bno_config::left_shin] = path_config::left_moustache_channel;
    muxChannels[bno_config::right_shin] = path_config::right_moustache_channel;
    muxChannels[bno_config::exo_back] = path_config::back_channel;
    muxChannels[bno_config::mobo] = path_config::mobo_channel;

    i2cAddresses[bno_config::left_thigh] = path_config::address_1;
    i2cAddresses[bno_config::right_thigh] = path_config::address_1;
    i2cAddresses[bno_config::left_shin] = path_config::address_2;
    i2cAddresses[bno_config::right_shin] = path_config::address_2;
    i2cAddresses[bno_config::exo_back] = path_config::address_1;
    i2cAddresses[bno_config::mobo] = path_config::address_2;

    bufferIndexLeft = 0;
    bufferIndexRight = 0;

    for(int i = 0; i < bno_config::buffer_size; i++)
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

    if (debug::bno) Serial.print("===== BnoHandler STARTING =====\n");

    for (int i = 0; i < bnoDevices.size(); i++){
        mux.selectChannel(muxChannels[i]);

        const bool isConnected = bnoDevices[i].begin(i2cAddresses[i], Wire);
        setupReports(i);
        bnoConnected[i] = isConnected;

        if (isConnected){
            connected++;
        }

        if (debug::bno) {
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
    for (int i = 0; i < bno_config::amount; i++){
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

            if (bno_config::mobo == i)
                BNOAngles[i] = -1*(degrees(atan2(gY, gX)) + 180);
            else
                BNOAngles[i] = degrees(atan2(gX, gY)) + 180;

            if (BNOAngles[i] > 180) BNOAngles[i] -= 360;
            else if (BNOAngles[i] <= -180) BNOAngles[i] += 360;

            if (bno_config::left_thigh == i
                || bno_config::left_shin == i)
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

void BnoHandler::getAngle(float angles[bno_config::amount])
{
    memcpy(angles, &BNOAngles, sizeof(BNOAngles));
}

void BnoHandler::updateBuffer(uint8_t position)
{
    const int16_t yAccel = getLinAccelYScaled(position);
    if (position==bno_config::left_shin){
        linAccelBufferLeft[bufferIndexLeft] = abs(yAccel) < (bno_config::accel_threshold + offset);
        if(bufferIndexLeft < (bno_config::buffer_size-1)) bufferIndexLeft++;
        else bufferIndexLeft = 0;
    }
    else if (position==bno_config::right_shin){
        linAccelBufferRight[bufferIndexRight] = abs(yAccel) < (bno_config::accel_threshold + offset);
        if(bufferIndexRight < (bno_config::buffer_size-1)) bufferIndexRight++;
        else bufferIndexRight = 0;
    }
}

void BnoHandler::getGroundedState(bool grounded[bno_config::nb_leg])
{
    float bufferAvg = 0;
    updateBuffer(bno_config::left_shin);
    for(int i=0; i<bno_config::buffer_size;i++)
    {
        bufferAvg += linAccelBufferLeft[i];
    }
    bufferAvg /= bno_config::buffer_size;
    grounded[bno_config::left_leg] = bufferAvg >= bno_config::accel_buffer_threshold;

    bufferAvg = 0;
    updateBuffer(bno_config::right_shin);
    for(int i=0; i<bno_config::buffer_size;i++)
    {
        bufferAvg += linAccelBufferRight[i];
    }
    bufferAvg /= bno_config::buffer_size;
    grounded[bno_config::right_leg] = bufferAvg >= bno_config::accel_buffer_threshold;
}

int16_t BnoHandler::getLinAccelYScaled(uint8_t position)
{
    const float y = linearAccelerations[bnoIndex(position)].y;
    return static_cast<int16_t>(y * 256.0f);
}

void BnoHandler::printName(uint8_t position){

    switch (position)
    {
    case bno_config::left_thigh:
        Serial.print("THIGH_L");
        break;
    case bno_config::right_thigh:
        Serial.print("THIGH_R");
        break;
    case bno_config::left_shin:
        Serial.print("TIBIA_L");
        break;
    case bno_config::right_shin:
        Serial.print("TIBIA_R");
        break;
    case bno_config::exo_back:
        Serial.print("EXO_BACK");
        break;
    case bno_config::mobo:
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