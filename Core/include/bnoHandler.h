#ifndef BNOHANDLER_H_
#define BNOHANDLER_H_
#include "SparkFun_BNO080_Arduino_Library.h"
#include "dataCore.h"
#include "multiplex.h"
#include "enums.h"
#include "define.h"
#include <array>
using namespace std;

// Class storing all BNOs and the multiplexer that they use
class BnoHandler {
    private:
        // Array of physical BNO08x instances, ordered by EnumBnoPosition
        array<BNO080, 6> bnoDevices;
        // Latest report values for each BNO (all sensors stored in one struct per device)
        array<BnoData_t, 6> bnoData;
        // BNO connection state
        array<bool, 6> bnoConnected;
        // Mux channel for each BNO
        array<uint8_t, 6> muxChannels;
        // I2C address for each BNO
        array<uint8_t, 6> i2cAddresses;
        // BNO orientation angles, stored in degrees and indexed by EnumBnoPosition
        array<float, 6> angles;
        // Multiplexer used to switch between BNOs
        Multiplex mux;
        // Time of last update, based on millis()
        long last_update = 0;
        // Instance of dataCore
        DataCore& dataCore = DataCore::getInstance();

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
         * @brief Write new value into buffer and increment pointer
         * @param position EnumBnoPosition of the part
         */
        void updateBuffer(EnumBnoPosition position);

        /**
         * @brief Reset BNO data structur
         * 
         * @param position BNO to reset
         */
        void resetData(EnumBnoPosition position);

        /**
         * @brief Configure all required sensor reports for one BNO
         * @param position EnumBnoPosition of the BNO to configure
         * @return true if all required reports are enabled
         * @return false if any report setup fails
         */
        bool setupReports(EnumBnoPosition position);

        /**
         * @brief Probe one BNO over I2C and update its connection state
         * @param position EnumBnoPosition of the BNO to check
         * @return true if the BNO responds on I2C
         * @return false if no response is received
         */
        bool checkIfConnected(EnumBnoPosition position);

        /**
         * @brief Compute pitch angle from the latest rotation-vector quaternion
         * @param position EnumBnoPosition of the BNO
         * @return Pitch in degrees
         */
        float getPitchDegrees(EnumBnoPosition position);

        /**
         * @brief Get Y axis linear acceleration scaled to fixed-point format
         * @param position EnumBnoPosition of the BNO
         * @return Linear acceleration on Y axis, scaled by 256
         */
        int16_t getLinAccelYScaled(EnumBnoPosition position);

    public:
        int offset = 0;
        /**
        * @brief Constructor, sets up individual BNOs. Data aquisition is not started.
        */
        BnoHandler();

        /**
        * @brief Read angles and store them in dataCore
        */
        void read();

        /**
        * @brief Constructor, sets up individual BNOs. Data aquisition is not started.
        * @return True if at least one BNO is connected
        */
        bool begin();

        /**
         * @brief Request data from all BNOs and compute angles between parts
         */
        void requestData();

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
         * @brief Compute the relative angles at the joints
         */
        void computeAngles();

        /**
         * @brief Get the relative angle of a part
         * @param position EnumBnoPosition of the part
         * @return Angle in degrees
         */
        float getValAngle(EnumBnoPosition position);

        /**
         * @brief Get the BNO data of a part
         * @param position EnumBnoPosition of the part
         * @return Latest rotation-vector report for the part
         */
        BnoData_t getBNOData(EnumBnoPosition position);

        /**
         * @brief Get direct pointer to latest sensor storage for a BNO
         * @param position EnumBnoPosition of the part
         * @return Pointer to internal storage
         */
        BnoData_t* getBNODataPointer(EnumBnoPosition position);

        /**
         * @brief Compute linear acceleration from an average on linAccelBuffer
         * @param position EnumBnoPosition of the part
         * @return GroundedStatus
         */
        bool getLinAccel(EnumBnoPosition position);

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

        /**
         * @brief Print left/right grounded state and current threshold
         */
        void printGroundState();
};
#endif