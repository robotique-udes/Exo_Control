#ifndef BIOGENIUSBNO_H_
#define BIOGENIUSBNO_H_
#include "structs.h"
#include "multiplex.h"
#include "enums.h"
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

/**
 * Implementation of a single BNO_085, read using I2C through a multiplexer.
 *
 * The class stores the latest data from the BNO, providing access to them
 * through getter functions. beginTracking must be used to initialize the BNO
 * and requestData must be used to update the data.
 */
class BNO {
    private:
        // Latest data from BNO
        BNOStruct data;
        // Port # on which this BNO is connected
        int muxAddress;
        // Position of the BNO on the exo
        EnumBnoPosition position;
        // Pointer to the multiplexer that the BNO is connected to
        Multiplex* muxPtr;
        // I2C address of the BNO
        int i2cAddress = 0x4B;
        // Generic implementation of request setup (see raw.cpp)
        void beginTransmission(uint8_t reportType);
        // Successful connection was established previously
        bool connected = false;
        // Ensure channel is available
        void ensureReadAvailable(int16_t length);
        // Computed the Euler angles from the quaternion
        void updateEuler();
        // Sets all fields to 0
        void resetDataValues();

    public:
        /**
        * @brief Constructor with the given parameters. 
        * The BNO is *not* initialized
        *
        * @param position position of the BNO on the exo, see EnumBnoPosition
        * @param muxAddress port # on which this BNO is connected
        * @param muxPtr pointer to the multiplexer that the BNO is connected to
        * @param i2cAddress I2C address of the BNO
        */
        BNO(EnumBnoPosition position, int muxAddress, Multiplex* muxPtr, int i2cAddress);
        ~BNO();

        /**
        * Request latest data, store values in data structure.
        * @return true if data was successfully requested
        */
        bool requestData();

        /**
        * @brief Send statup commands to start storing data.
        * @return true if tracking was successfully started
        */
        bool beginTracking();

        /**
        * Sends a simple packet to check connection status.
        * @return true if the BNO responded
        */
        bool checkIfConnected();

        /**
        * Prints to serial the name of the BNO, without a newline.
        */
        void printName();

        /**
        * Returns the quaternion datastructure. 
        * @return [w, x, y, z]
        */
        array<int16_t, 4> getQuat();

        /**
        * Returns the euler datastructure, in degrees by default.
        * 
        * @param radians if true, returns the angles in radians
        * @return [x, y, z] in degrees or radians
        */
        array<float, 3> getEuler(bool radians = false);

        /**
        * Returns the acceleration datastructure.
        * @return [x, y, z]
        */
        array<int16_t, 3> getAcceleration();

        /**
        * Returns the gyroscope datastructure.
        * @return [x, y, z]
        */
        array<int16_t, 3> getGyro();

        /**
        * Returns the magnetometer datastructure.
        * @return [x, y, z]
        */
        array<int16_t, 3> getMag();

        /**
        * Returns the linear acceleration datastructure.
        * @return [x, y, z]
        */
        array<int16_t, 3> getLinAcceleration();

        /**
        * Returns the complete data structure.
        */
        BNOStruct getData();

        /**
        * Returns the time of the latest data.
        */
        int16_t getTime();
};
#endif