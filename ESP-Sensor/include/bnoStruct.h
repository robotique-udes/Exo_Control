using namespace std;
#include <vector>
#include <stdint.h>

struct BNOStruct
{
    vector<int16_t> quat{int16_t 4}; // Stored as W, X, Y, Z
    vector<int16_t> euler{int16_t 3}; // Stored as yaw, pitch, roll as radians
    vector<int16_t> acceleration{int16_t 3}; // Stored as X, Y, Z
    vector<int16_t> gyro{int16_t 3}; // Stored as X, Y, Z
    vector<int16_t> mag{int16_t 3}; // Stored as X, Y, Z
    vector<int16_t> lin_acceleration(int16_t 3); // Stored as X, Y, Z
    int16_t time;
};