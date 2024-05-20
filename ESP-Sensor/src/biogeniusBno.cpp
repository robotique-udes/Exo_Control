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
}

BNO::BNO(enumIMU position, int muxAddress, Multiplex* muxPtr) {
    this->position = position;
    this->muxAddress = muxAddress;
    this->muxPtr = muxPtr;
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

BNO::BNO(enumIMU position) {
    this->position = position;
    // MUX not used in this version
    this->resetDataValues();

    this->i2cAddress = 0x4A;

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

bool BNO::isConnected() {
    return this->connected;
}

array<float, 3> BNO::getEuler(bool degrees) {
    // TODO - Check if values need to be converted depending on BNO placement
    // TODO check to implement degrees return this->data.euler * (degrees ? RAD_TO_DEG : 1.0);
    if (degrees) {
        return {this->data.euler[0] * RAD_TO_DEG, this->data.euler[1] * RAD_TO_DEG, this->data.euler[2] * RAD_TO_DEG};
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

// Start tracking of BNO, returns true if successful
bool BNO::beginTracking() {
    Serial.print("beginTracking of IMU ");
    this->printName();
    Serial.print(" w/address 0x");
    Serial.println(this->i2cAddress, HEX);

    this->muxPtr->selectChannel(this->muxAddress);
        
    // request acc reports, see 6.5.4
    static const uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, ACC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);  Wire.write(cmd_acc, sizeof(cmd_acc));  Wire.endTransmission();

    // request gyro reports, see 6.5.4
    static const uint8_t cmd_gyro[] = {21, 0, 2, 0, 0xFD, GYRO_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);  Wire.write(cmd_gyro, sizeof(cmd_gyro));  Wire.endTransmission();

    // request magneto reports, see 6.5.4
    static const uint8_t cmd_mag[]  = {21, 0, 2, 0, 0xFD, MAG_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);  Wire.write(cmd_mag, sizeof(cmd_mag));  Wire.endTransmission();

    // request linear acc reports, see 6.5.4
    static const uint8_t cmd_lac[]  = {21, 0, 2, 0, 0xFD, LAC_REPORT,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);  Wire.write(cmd_lac, sizeof(cmd_lac));  Wire.endTransmission();

    // request quaternion reports, see 6.5.4
    static const uint8_t cmd_quat[] = {21, 0, 2, 0, 0xFD, QUAT_REPORT, 0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    Wire.beginTransmission(this->i2cAddress);  Wire.write(cmd_quat, sizeof(cmd_quat));  Wire.endTransmission();

    Serial.println("Reports requested");

    // Wait for BNO to return non-zero quaternions
    // Number of tries is a magic number, may need to be increased
    bool status = false;

    for (uint8_t i=0; i<20 && !status; i++) {
        requestData();
        if (!(!this->data.quat[0] && !this->data.quat[1] && !this->data.quat[2] && !this->data.quat[3])) {
            Serial.println("First data valid!");

            Serial.print("Quat: ");
            Serial.print(this->data.quat[0]); Serial.print(" ");
            Serial.print(this->data.quat[1]); Serial.print(" ");
            Serial.print(this->data.quat[2]); Serial.print(" ");
            Serial.println(this->data.quat[3]);

            status = true;
            Serial.println("Status true!");
        }
    } 

    this->connected = status;
    return status;
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

bool BNO::requestData() {
    int16_t length;
    uint8_t channel __attribute__((unused));
    uint8_t seqnum  __attribute__((unused));

    // TODO add back change to multiplexer
    this->muxPtr->selectChannel(this->muxAddress);

    Serial.println("\nInitial SHTP header");
    // Parse SHTP header
    Wire.requestFrom(this->i2cAddress,4+1);       // read 4-byte SHTP header and first byte of cargo
    Serial.println("First requestFrom done");

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