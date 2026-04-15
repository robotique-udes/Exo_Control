#ifndef BNOHANDLER_H_
#define BNOHANDLER_H_
#include "Adafruit_BNO08x.h"
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
        array<Adafruit_BNO08x, 6> bnoDevices;
        // Latest report values for each BNO
        array<sh2_SensorValue_t, 6> bnoRotation;
        array<sh2_SensorValue_t, 6> bnoAccel;
        array<sh2_SensorValue_t, 6> bnoLinAccel;
        array<sh2_SensorValue_t, 6> bnoGyro;
        array<sh2_SensorValue_t, 6> bnoMag;
        // BNO connection state
        array<bool, 6> bnoConnected;
        // Mux channel for each BNO
        array<uint8_t, 6> muxChannels;
        // I2C address for each BNO
        array<uint8_t, 6> i2cAddresses;
        // Array of the angles between the parts, stored in degrees
        array<float, 9> angles;
        // Multiplexer used to switch between BNOs
        Multiplex mux;
        // Reused event buffer for BNO08x reports
        sh2_SensorValue_t sensorValue;
        // Time of last update, based on millis()
        long last_update = 0;
        // Instance of dataCore
        DataCore& dataCore = DataCore::getInstance();

        int bufferIndexLeft;
        float linAccelBufferLeft[BUFFER_SIZE];
        int bufferIndexRight;
        float linAccelBufferRight[BUFFER_SIZE];

        /**
         * @brief Write new value into buffer and increment pointer
         * @param position EnumBnoPosition of the part
         */
        void updateBuffer(EnumBnoPosition position);
        void resetData(EnumBnoPosition position);
        bool setupReports(int index);
        bool checkIfConnected(int index);
        float getPitchDegrees(EnumBnoPosition position);
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
        void printBNOsData(int startIndex = 0, int endIndex = 4);
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
        float getValAngle(EnumBnoAngle position);

        /**
         * @brief Get the BNO data of a part
         * @param position EnumBnoPosition of the part
         * @return Latest rotation-vector report for the part
         */
        sh2_SensorValue_t getBNOData(EnumBnoPosition position);

        sh2_SensorValue_t* getBNODataPointer(EnumBnoPosition position);

        /**
         * @brief Compute linear acceleration from an average on linAccelBuffer
         * @param position EnumBnoPosition of the part
         * @return GroundedStatus
         */
        bool getLinAccel(EnumBnoPosition position);

        void printName(EnumBnoAngle position);

        void printBNOData(EnumBnoPosition position);

        void printGroundState();
};
#endif