#include "biogeniusBno.h"

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

BNO::ensureReadAvailable(int16_t length)
{
    // Ensure a read byte is available, if necessary reread and discard 4-byte SHTP header, then read as much length as possible
    if (!this->MUXWire->available()) {
        this->MUXWire->requestFrom(this->i2cAddress,4+length),
            this->MUXWire.read(),
            this->MUXWire.read(),
            this->MUXWire.read(),
            this->MUXWire.read();
    }
}

BNO::MUXWire() {
    return this->muxPtr->getWire();
}

BNO::getQuat() {
    return this->data.quat;
}

BNO::getAcceleration() {
    return this->data.acceleration;
}

BNO::getGyro() {
    return this->data.gyro;
}

BNO::getMag() {
    return this->data.mag;
}

BNO::getLinAcceleration() {
    return this->data.lin_acceleration;
}

BNO::getTime() {
    return this->data.time;
}

BNO::getEuler(bool degrees = false) {
    // TODO - Check if values need to be converted depending on BNO placement
    return this->data.euler * (degrees ? RAD_TO_DEG : 1.0)
}

BNO::~BNO() {};

BNO::beginTransmission(int reportType) {
    // Manual write to start report of given type
    static const uint8_t cmd_acc[]  = {21, 0, 2, 0, 0xFD, reportType,  0, 0, 0, (SENSOR_US>>0)&255, (SENSOR_US>>8)&255, (SENSOR_US>>16)&255, (SENSOR_US>>24)&255, 0, 0, 0, 0, 0, 0, 0, 0};
    this->MUXWire->beginTransmission(this->i2cAddress);
    this->MUXWire->write(cmd_acc, sizeof(cmd_acc));
    this->MUXWire->endTransmission();
}

BNO::begin() {
    if (this->muxPtr->getCurrentChannel() != this->muxAddress) {
        this->muxPtr->selectChannel(this->muxAddress);
    }

    // Start reports, maybe toggling those that are unused could help if latency is a factor
    beginTransmission(QUAT_REPORT);
    beginTransmission(GYRO_REPORT);
    beginTransmission(TIME_REPORT);

    // Wait for BNO to return non-zero quaternions
    // Number of tries is a magic number, may need to be increased
    bool status = false;

    for (uint8_t i=0; i<1000 || !status; i++) {
        requestData();
        if (this->data.quat[0] ||
            this->data.quat[1] ||
            this->data.quat[2] ||
            this->data.quat[3]) {
            status = true;
        }
    } 

    return status;
}

BNO::updateEuler() {
    float sqw = sq(this->data.quat[0]);
    float sqx = sq(this->data.quat[1]);
    float sqy = sq(this->data.quat[2]);
    float sqz = sq(this->data.quat[3]);

    // Yaw
    this->data->euler[0] = atan2(2.0 * 
        (this->data.quat[1] * this->data.quat[2] + this->data.quat[3] * this->data.quat[0]),
        (sqx - sqy - sqz + sqw));

    // Pitch
    this->data->euler[1] = asin(-2.0 * 
        (this->data.quat[1] * this->data.quat[3] - this->data.quat[2] * this->data.quat[0]) / 
        (sqx + sqy + sqz + sqw));
    
    // Roll
    this->data->euler[2] = atan2(2.0 * 
        (this->data.quat[2] * this->data.quat[3] + this->data.quat[1] * this->data.quat[0]),
        (-sqx - sqy + sqz + sqw));
}

BNO::requestData() {
    int16_t length;
    uint8_t channel __attribute__((unused));
    uint8_t seqnum  __attribute__((unused));

    if (muxPtr->getCurrentChannel() != muxAddress) {
        muxPtr->selectChannel(muxAddress);
    }

    // Parse SHTP header
    this->MUXWire->requestFrom(this->i2cAddress,4+1);       // read 4-byte SHTP header and first byte of cargo

    length  = this->MUXWire.read();     // length LSB
    length |= (this->MUXWire.read() & 0x7F) << 8;  // length MSB (ignore continuation flag)
    channel = this->MUXWire.read();     // channel number
    seqnum  = this->MUXWire.read();     // sequence number (ignore)
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

        buf[n++] = this->MUXWire->read();  // first byte of report
        length--;

        // known reports
        if (channel==3 && buf[0]==TIME_REPORT && length >= 5-1)
        {
            for (uint8_t n=1; n<5; n++)       // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = this->MUXWire->read();
                length--;
            }
            continue;
        }

        if (channel==3 && buf[0]==ACC_REPORT && length >= 10-1)
        {
            for (uint8_t n=1; n<10; n++)      // read remainder of report
            {
                this->ensureReadAvailable(length);
                buf[n] = this->MUXWire->read();
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
                buf[n] = this->MUXWire->read();
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
                buf[n] = this->MUXWire->read();
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
                buf[n] = this->MUXWire->read();
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
                buf[n] = this->MUXWire->read();
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
            this->MUXWire->read();
            length--;
        }
        continue;
    }

    // Potentially check if hashed data has changed to validate
    // that the data has actually changed, if not return false
    return true;
}