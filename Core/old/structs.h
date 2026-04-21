#ifndef STRUCTS_H
#define STRUCTS_H
using namespace std;
#include <array>
#include <stdint.h>

/**
 * Wrapper for the data from the BNO_085.
 */
struct BNOStruct
{
    array<int16_t, 4> quat; // Stored as W, X, Y, Z
    array<float, 3> euler; // Stored as yaw, pitch, roll as radians
    array<int16_t, 3> acceleration; // Stored as X, Y, Z
    array<int16_t, 3> gyro; // Stored as X, Y, Z
    array<int16_t, 3> mag; // Stored as X, Y, Z
    array<int16_t, 3> lin_acceleration; // Stored as X, Y, Z
    int16_t time; // Timestamp of the data, based on the BNO's internal clock
};

#endif