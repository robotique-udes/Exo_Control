#ifndef BIOGENIUSIMU_H_
#define BIOGENIUSIMU_H_
#include "biogeniusBno.h"
#include "multiplex.h"
#include "enums.h"
#include "define.h"
#include <array>
using namespace std;

// Class storing all the IMU classes and the multiplexer that they use
class IMU {
    private:
        array<BNO *, 5> BNOs;
        array<float, 5> angles;
        Multiplex mux;
        long last_update = 0;

    public:
        IMU();
        bool begin();
        void requestData();
        void printBNOs(int startIndex = 0, int endIndex = 5);
        void computeAngles(); // Computes angles of each part relative to each other
        float getValAngle(enumIMU position); // Returns the relative angle of a part
        BNOStruct getBNOData(enumIMU position);

};
#endif