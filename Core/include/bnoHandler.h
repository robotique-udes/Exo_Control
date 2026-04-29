#ifndef BNOHANDLER_H_
#define BNOHANDLER_H_
#include "SparkFun_BNO080_Arduino_Library.h"
#include "multiplex.h"
#include "define.h"
#include <array>
using namespace std;

#define NUMBER_OF_BNO 6

/**
 * @brief Physical BNO locations on the exoskeleton.
 */
enum class EnumBnoPosition
{
    THIGH_L    =   0,
    THIGH_R    =   1,
    TIBIA_L    =   2,
    TIBIA_R    =   3,
    EXO_BACK   =   4,
    MOBO       =   5
};

/**
 * @brief Enum indexes used to expose angle outputs.
 */
enum class AngleOutput : uint8_t
{
    HipLeft,
    HipRight,
    KneeLeft,
    KneeRight,
    Back
};

/**
 * @brief Aggregated joint angles returned by `getAngle()`.
 */
typedef struct
{
    float hipLeft;
    float hipRight;
    float KneeLeft;
    float KneeRight;
    float back;

} angleOutput_t;
/**
 * @brief Result of ground-contact detection for left / right legs.
 */
typedef struct
{
    bool isLeftGrounded;
    bool isRightGrounded;

} groundedOutput_t;

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
        array<BNO080, NUMBER_OF_BNO> bnoDevices;
        // BNO connection state
        array<bool, NUMBER_OF_BNO> bnoConnected;
        // Mux channel for each BNO
        array<uint8_t, NUMBER_OF_BNO> muxChannels;
        // I2C address for each BNO
        array<uint8_t, NUMBER_OF_BNO> i2cAddresses;
        // Linear acceleration for each bno
        array<linearAcceleration_t, NUMBER_OF_BNO> linearAccelerations;
        // Angle output (HipLeft, HipRight, KneeLeft, KneeRight, Back)
        array<float, 5> angles;
        // Multiplexer used to switch between BNOs
        Multiplex mux;
        // Time of last update, based on millis()
        long last_update = 0;

        int bufferIndexLeft;
        float linAccelBufferLeft[BUFFER_SIZE];
        int bufferIndexRight;
        float linAccelBufferRight[BUFFER_SIZE];

        /**
         * @brief Convert BNO enum position to array index
         * @param position EnumBnoPosition value
         * @return Zero-based index matching internal storage order
         */
        static constexpr size_t bnoIndex(EnumBnoPosition position) {
            return static_cast<size_t>(position);
        }

        /**
         * @brief Write latest linear-accel Y value into the circular buffer for the
         * specified `position` and advance the write pointer.
         * @param position BNO position to update buffer for
         */
        void updateBuffer(EnumBnoPosition position);

        /**
         * @brief Configure the reports/features required from the BNO080 at `position`.
         * @param position BNO to configure
         */
        void setupReports(EnumBnoPosition position);

        /**
         * @brief Probe a device by selecting the proper mux channel and attempting
         * an I2C transmission to its configured address.
         * @param position BNO to probe
         * @return true if the device acknowledged on I2C
         */
        bool checkIfConnected(EnumBnoPosition position);

        /**
         * @brief Read the buffered linear-acceleration Y for `position` and
         * return it in fixed-point Q8 format (value * 256).
         * @param position BNO position
         * @return Scaled Y linear-acceleration (int16_t)
         */
        int16_t getLinAccelYScaled(EnumBnoPosition position);
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
        angleOutput_t getAngle();

        /**
         * @brief Compute and return left/right grounded state using the
         * moving-average linear-accel buffers.
         * @return `groundedOutput_t` with booleans for left/right ground contact.
         */
        groundedOutput_t getGroundedState();

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
        void printName(EnumBnoPosition position);

        /**
         * @brief Print accelerometer and linear-acceleration data for one BNO
         * @param position EnumBnoPosition of the BNO to print
         */
        void printBNOData(EnumBnoPosition position);

};
#endif