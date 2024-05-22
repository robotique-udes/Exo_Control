#include "biogeniusBno.h"
#include "Arduino.h"
#include <array>
#include <Wire.h>
#include "define.h"
using namespace std;

BNO::BNO(enumIMU position, int muxAddress, Multiplex* muxPtr, int i2cAddress) {
    this->position = position;
    this->muxAddress = muxAddress;
    this->muxPtr = muxPtr;
    this->i2cAddress = i2cAddress;
    this->resetDataValues();
}

BNO::BNO(enumIMU position, int muxAddress, Multiplex* muxPtr) {
    this->position = position;
    this->muxAddress = muxAddress;
    this->muxPtr = muxPtr;
    this->resetDataValues();
}

// Sets to 0 all structure values
void BNO::resetDataValues() {
    this->data.acceleration[0] = 0;
    this->data.acceleration[1] = 0;
    this->data.acceleration[2] = 0;

    this->data.quat[0] = 0;
    this->data.quat[1] = 0;
    this->data.quat[2] = 0;
    this->data.quat[3] = 0;

    this->data.euler[0] = 0;
    this->data.euler[1] = 0;
    this->data.euler[2] = 0;

    this->data.gyro[0] = 0;
    this->data.gyro[1] = 0;
    this->data.gyro[2] = 0;

    this->data.lin_acceleration[0] = 0;
    this->data.lin_acceleration[1] = 0;
    this->data.lin_acceleration[2] = 0;
    this->data.mag[0] = 0;
    this->data.mag[1] = 0;
    this->data.mag[2] = 0;

    this->data.time = 0;
}

void BNO::printName() {
    switch (this->position)
    {
    case enumIMU::EXO_BACK:
        Serial.print("Back");
        break;
    case enumIMU::HIP_L:
        Serial.print("Hip Left");
        break;
    case enumIMU::HIP_R:

        Serial.print("Hip Right");
        break;
    case enumIMU::KNEE_L:
        Serial.print("Knee Left");
        break;
    case enumIMU::KNEE_R:
        Serial.print("Knee Right");
        break;
    default:
        Serial.print("Unknown");
        break;
    }
}

void BNO::ensureReadAvailable(int16_t length)
{
    // Ensure a read byte is available, if necessary reread and discard 4-byte SHTP header, then read as much length as possible
    if (!Wire.available()) {
        Wire.requestFrom(this->i2cAddress,4+length),
            Wire.read(),
            Wire.read(),
            Wire.read(),
            Wire.read();
    }
}

array<int16_t, 4> BNO::getQuat() {
    return this->data.quat;
}

array<int16_t, 3> BNO::getAcceleration() {
    return this->data.acceleration;
}

array<int16_t, 3> BNO::getGyro() {
    return this->data.gyro;
}

array<int16_t, 3> BNO::getMag() {
    return this->data.mag;
}

array<int16_t, 3> BNO::getLinAcceleration() {
    return this->data.lin_acceleration;
}

int16_t BNO::getTime() {
    return this->data.time;
}

// "Ping" the BNO to check if it is connected
bool BNO::checkIfConnected() {
    this->muxPtr->selectChannel(this->muxAddress);
    Wire.beginTransmission(this->i2cAddress);
    return Wire.endTransmission() == 0;
}

// Returns the Euler angles of the BNO, in degrees by default
array<float, 3> BNO::getEuler(bool radians) {
    if (!radians) {
        return {(float)(this->data.euler[0] * RAD_TO_DEG),
                (float)(this->data.euler[1] * RAD_TO_DEG),
                (float)(this->data.euler[2] * RAD_TO_DEG)};
    } else {
        return this->data.euler;
    }
}

BNO::~BNO() {};

void BNO::beginTransmission(uint8_t reportType) {
    // Manual write to start report of given type
    uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, reportType,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);
    Wire.write(cmd_acc, sizeof(cmd_acc));
    Wire.endTransmission();
}

BNOStruct BNO::getData() {
    return this->data;
}

void BNO::updateEuler() {
    float sqw = sq(this->data.quat[0]);
    float sqx = sq(this->data.quat[1]);
    float sqy = sq(this->data.quat[2]);
    float sqz = sq(this->data.quat[3]);

    // Yaw
    this->data.euler[0] = atan2(2.0 * 
        (this->data.quat[1] * this->data.quat[2] + this->data.quat[3] * this->data.quat[0]),
        (sqx - sqy - sqz + sqw));

    // Pitch
    this->data.euler[1] = asin(-2.0 * 
        (this->data.quat[1] * this->data.quat[3] - this->data.quat[2] * this->data.quat[0]) / 
        (sqx + sqy + sqz + sqw));
    
    // Roll
    this->data.euler[2] = atan2(2.0 * 
        (this->data.quat[2] * this->data.quat[3] + this->data.quat[1] * this->data.quat[0]),
        (-sqx - sqy + sqz + sqw));
}

// Try to request data from BNO, return true if successful
bool BNO::requestData() {

    int16_t length;
    uint8_t channel __attribute__((unused));
    uint8_t seqnum  __attribute__((unused));

    this->muxPtr->selectChannel(this->muxAddress);

    if (!this->checkIfConnected()) {
        this->resetDataValues();
        this->connected = false;
        return false;
    }

    // BNO was not previously connected, send setup commands
    // or if quat[0] is 0, then we need to reinitialize
    if (!this->connected || this->data.quat[0] == 0) {
        // TO TEST if power was disconnected, the BNO seems to need the reset pin, to test
        this->beginTransmission(ACC_REPORT);
        this->beginTransmission(GYRO_REPORT);
        this->beginTransmission(MAG_REPORT);
        this->beginTransmission(LAC_REPORT);
        this->beginTransmission(QUAT_REPORT);
        this->beginTransmission(TIME_REPORT);
        this->resetDataValues();
        this->connected = true;
    }

    // Parse SHTP header
    Wire.requestFrom(this->i2cAddress,4+1);       // read 4-byte SHTP header and first byte of cargo

    length  = Wire.read();     // length LSB
    length |= (Wire.read() & 0x7F) << 8;  // length MSB (ignore continuation flag)
    channel = Wire.read();     // channel number
    seqnum  = Wire.read();     // sequence number (ignore)
    length -= 4;                        // done reading SHTP Header
    if (length <= 0 || length > 1000)   // if null/bad/degeneratje SHTP header
    {
        return false;
    }

    while (length)                       // while more reports in cargo
    {
        uint8_t buf[20];                    // report buffer, big enough for longest interesting report (uninteresting reports will be ignored)
        uint16_t n = 0;                     // index into report buffer

        this->ensureReadAvailable(length);

        buf[n++] = Wire.read();  // first byte of report
        length--;

        // known reports
        if (channel==3 && buf[0]==TIME_REPORT && length >= 5-1)
        {
            for (uint8_t n=1; n<5; n++)       // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }
            continue;
        }

        if (channel==3 && buf[0]==ACC_REPORT && length >= 10-1)
        {
            for (uint8_t n=1; n<10; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }
            this->data.acceleration[0] = *(int16_t*)&buf[4]; // X
            this->data.acceleration[1] = *(int16_t*)&buf[6]; // Y
            this->data.acceleration[2] = *(int16_t*)&buf[8]; // Z
            continue;
        }

        if (channel==3 && buf[0]==GYRO_REPORT && length >= 10-1)
        {
            for (uint8_t n=1; n<10; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }
            this->data.gyro[0] = *(int16_t*)&buf[4]; // X
            this->data.gyro[1] = *(int16_t*)&buf[6]; // Y
            this->data.gyro[2] = *(int16_t*)&buf[8]; // Z
            continue;
        }

        if (channel==3 && buf[0]==MAG_REPORT && length >= 10-1)
        {
            for (uint8_t n=1; n<10; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }

            this->data.mag[0] = *(int16_t*)&buf[4]; // X
            this->data.mag[1] = *(int16_t*)&buf[6]; // Y
            this->data.mag[2] = *(int16_t*)&buf[8]; // Z
            continue;
        }

        if (channel==3 && buf[0]==LAC_REPORT && length >= 10-1)
        {
            for (uint8_t n=1; n<10; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }
            this->data.lin_acceleration[0] = *(int16_t*)&buf[4]; // X
            this->data.lin_acceleration[1] = *(int16_t*)&buf[6]; // Y
            this->data.lin_acceleration[2] = *(int16_t*)&buf[8]; // Z
            continue;
        }

        if (channel==3 && buf[0]==QUAT_REPORT && length >= 14-1)
        {
            for (uint8_t n=1; n<14; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = Wire.read();
                length--;
            }

            this->data.quat[0] = *(int16_t*)&buf[10]; // W
            this->data.quat[1] = *(int16_t*)&buf[4];  // X
            this->data.quat[2] = *(int16_t*)&buf[6];  // Y
            this->data.quat[3] = *(int16_t*)&buf[8];  // Z

            // Update Euler angles
            this->updateEuler();

            continue;
        }

        // unknown reports
        while (length)                      // discard remainder of cargo (shouldn't happen very often)
        {
            this->ensureReadAvailable(length);
            Wire.read();
            length--;
        }
        continue;
    }

    // Potentially check if hashed data has changed to validate
    // that the data has actually changed, if not return false
    return true;
}