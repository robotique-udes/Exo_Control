#ifndef BIOGENIUSIMU_H_
#define BIOGENIUSIMU_H_
#include "biogeniusBno.h"
#include "multiplex.h"
#include "enum.h"
#include <array>
using namespace std;

// Class storing all the IMU classes and the multiplexer that they use
class IMU {
    private:
        array<BNO *, 5> BNOs;
        Multiplex mux;
        long last_update = 0;

    public:
        IMU();
        IMU(TwoWire* wireUsed);
        bool begin();
        void requestData();
        int16_t getValAngle(enumIMU position);
        BNOStruct getBNOData(enumIMU position);

};
#endif