#ifndef BNOHANDLER_H_
#define BNOHANDLER_H_
#include "BNO_085.h"
#include "dataCore.h"
#include "multiplex.h"
#include "enums.h"
#include "define.h"
#include <array>
using namespace std;

// Class storing all BNOs and the multiplexer that they use
class BnoHandler {
    private:
        // Array of the physical BNOs, ordered by EnumBnoPosition
        array<BNO_085 *, 5> BNOs;
        // Array of the angles between the parts, stored in degrees
        array<float, 9> angles;
        // Multiplexer used to switch between BNOs
        Multiplex mux;
        // Time of last update, based on millis()
        long last_update = 0;
        // Instance of dataCore
        DataCore& dataCore = DataCore::getInstance();

    public:
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
        void printBNOsData(int startIndex = 0, int endIndex = 8);

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
         * @return BNOStruct of the part
         */
        BNOStruct getBNOData(EnumBnoPosition position);

        void printName(EnumBnoAngle position);
};
#endif