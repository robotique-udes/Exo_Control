#ifndef BIOGENIUSIMU_H_
#define BIOGENIUSIMU_H_
#include "biogeniusBno.h"
#include "multiplex.h"
#include "enum.h"
#include <vector>

class IMU {
    private: 
        std::vector<BNO> BNOs;
        Multiplex mux;
        long last_update = 0;

    public:
        IMU();
        bool begin();
        void requestData();
        double getValAngle(enumIMU position);
        BNOStruct getBNOData(enumIMU position);

}