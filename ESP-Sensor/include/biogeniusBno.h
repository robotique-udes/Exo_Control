#ifndef BIOGENIUSBNO_H_
#define BIOGENIUSBNO_H_
#include "bnoStruct.h"
#include "multiplex.h"
#include "enumIMU.h"
#include <Wire.h>
#include <array>
using namespace std;

#define SENSOR_US   10000L      // time between sensor reports, microseconds, 10000L is 100 Hz, 20000L is 50 Hz
#define ACC_REPORT   0x01   // accel report, see 6.5.9
#define GYRO_REPORT  0x02   // gyro report, see 6.5.13
#define MAG_REPORT   0x03   // magneto report, see 6.5.16
#define LAC_REPORT   0x04   // linear accel report, see 6.5.10
#define QUAT_REPORT  0x05   // quaternion report, see 6.5.18
#define TIME_REPORT  0xFB   // time report, see 7.2.1

#define DEBUG_ACC   false
#define DEBUG_GYRO  false
#define DEBUG_MAG   false
#define DEBUG_LAC   false
#define DEBUG_QUAT  true
#define DEBUG_TIME  false

class BNO {
    private:
        BNOStruct data; // Latest data from BNO
        int muxAddress; // Port # on which this BNO is connected
        enumIMU position; // Position of the BNO on the exo
        Multiplex* muxPtr; // Pointer to the multiplexer that the BNO is connected to
        int i2cAddress = 0x4B; // I2C address of the BNO
        void beginTransmission(uint8_t reportType); // Generic implementation of request setup (see raw.cpp)
        bool connected = false; // Successful connection was established previously
        void ensureReadAvailable(int16_t length);
        void updateEuler();
        void resetDataValues(); // Sets all fields to 0

    public:
        BNO(enumIMU position, int muxAddress, Multiplex* muxPtr);
        BNO(enumIMU position, int muxAddress, Multiplex* muxPtr, int i2cAddress);
        ~BNO();
        bool requestData(); // Request data from BNO, stores values in data structure
        bool beginTracking();
        bool checkIfConnected();
        void printName();
        array<int16_t, 4> getQuat();
        array<float, 3> getEuler(bool degrees = false);
        array<int16_t, 3> getAcceleration();
        array<int16_t, 3> getGyro();
        array<int16_t, 3> getMag();
        array<int16_t, 3> getLinAcceleration();
        BNOStruct getData();
        int16_t getTime();
};
#endif