#ifndef BNOHANDLER_H_
#define BNOHANDLER_H_
#include "SparkFun_BNO080_Arduino_Library.h"
#include "multiplex.h"
#include "config.h"
#include <array>
using namespace std;

namespace bno_config = app::config::bnos;
namespace path_config = app::config::path;
namespace debug = app::config::debug;


/**
 * @brief Simple 3-axis linear-acceleration container.
 */
typedef struct
{
    float x;
    float y;
    float z;

} linearAcceleration_t;


/**
 * @brief Manager for multiple BNO080 devices connected through an I2C multiplexer.
 *
 * The class maintains an array of `BNO080` objects, per-device connection state,
 * recent linear acceleration samples and computed joint angles. It provides
 * methods to initialize devices, poll sensor data and query processed outputs.
 */
class BnoHandler {
    private:
        // Array of physical BNO08x instances, ordered by EnumBnoPosition
        array<BNO080, bno_config::amount> bnoDevices;
        // BNO connection state
        array<bool, bno_config::amount> bnoConnected;
        // Mux channel for each BNO
        array<uint8_t, bno_config::amount> muxChannels;
        // I2C address for each BNO
        array<uint8_t, bno_config::amount> i2cAddresses;
        // Linear acceleration for each bno
        array<linearAcceleration_t, bno_config::amount> linearAccelerations;
        // Angle output (HipLeft, HipRight, KneeLeft, KneeRight, Back)
        array<float, bno_config::amount> BNOAngles;
        // Multiplexer used to switch between BNOs
        Multiplex mux;
        // Time of last update, based on millis()
        long last_update = 0;

        int bufferIndexLeft;
        float linAccelBufferLeft[bno_config::buffer_size];
        int bufferIndexRight;
        float linAccelBufferRight[bno_config::buffer_size];

        /**
         * @brief Convert BNO enum position to array index
         * @param position EnumBnoPosition value
         * @return Zero-based index matching internal storage order
         */
        static constexpr size_t bnoIndex(uint8_t position) {
            return static_cast<size_t>(position);
        }

        /**
         * @brief Write latest linear-accel Y value into the circular buffer for the
         * specified `position` and advance the write pointer.
         * @param position BNO position to update buffer for
         */
        void updateBuffer(uint8_t position);

        /**
         * @brief Configure the reports/features required from the BNO080 at `position`.
         * @param position BNO to configure
         */
        void setupReports(uint8_t position);

        /**
         * @brief Probe a device by selecting the proper mux channel and attempting
         * an I2C transmission to its configured address.
         * @param position BNO to probe
         * @return true if the device acknowledged on I2C
         */
        bool checkIfConnected(uint8_t position);

        /**
         * @brief Read the buffered linear-acceleration Y for `position` and
         * return it in fixed-point Q8 format (value * 256).
         * @param position BNO position
         * @return Scaled Y linear-acceleration (int16_t)
         */
        int16_t getLinAccelYScaled(uint8_t position);
        int offset = 0;
        
        public:
        /**
         * @brief Construct the BNO handler and initialise internal buffers.
         *
         * This does not start data acquisition on the sensors; call `begin()` to
         * initialise hardware and enable reports.
         */
        BnoHandler();
        
        /**
         * @brief Poll all connected BNOs for available reports and update
         * internal `angles` and `linearAccelerations` buffers.
         */
        void requestData();

        /**
         * @brief Return computed joint angles.
         * @return `angleOutput_t` with current joint angles in degrees.
         */
        void getAngle(float angles[bno_config::amount]);

        /**
         * @brief Compute and return left/right grounded state using the
         * moving-average linear-accel buffers.
         * @return `groundedOutput_t` with booleans for left/right ground contact.
         */
        void getGroundedState(bool grounded[bno_config::nb_leg]);

        /**
         * @brief Initialise all BNO devices (select mux channel and call BNO begin).
         * @return true if at least one device was successfully initialised
         */
        bool begin();


        /**
         * @brief Print the name and connection status of all BNOs
         * @param startIndex Index of the first BNO to print (value of EnumBnoPosition)
         * @param endIndex Index of the last BNO to print (value of EnumBnoPosition)
         */
        void printBNOsStatus(int startIndex = 0, int endIndex = 4);

        /**
         * @brief Print accelerometer and linear-acceleration values for a BNO range
         * @param startIndex Index of the first BNO to print (value of EnumBnoPosition)
         * @param endIndex Index of the last BNO to print (value of EnumBnoPosition)
         */
        void printBNOsData(int startIndex = 0, int endIndex = 4);

        /**
         * @brief Print sensor values only for BNOs that are currently connected
         * @param startIndex Index of the first BNO to print (value of EnumBnoPosition)
         * @param endIndex Index of the last BNO to print (value of EnumBnoPosition)
         */
        void printConnectedBNOsData(int startIndex = 0, int endIndex = 4);

        /**
         * @brief Print human-readable name for one angle/part enum
         * @param position EnumBnoPosition value to print
         */
        void printName(uint8_t position);

        /**
         * @brief Print accelerometer and linear-acceleration data for one BNO
         * @param position EnumBnoPosition of the BNO to print
         */
        void printBNOData(uint8_t position);

};
#endif