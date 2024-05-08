#include <vector>

struct BNOStruct
{
    std::vector<int> quat{4}; // Stored as W, X, Y, Z
    std::vector<int> euler{3}; // Stored as yaw, pitch, roll as radians
    std::vector<int> acceleration{3}; // Stored as X, Y, Z
    std::vector<int> gyro{3}; // Stored as X, Y, Z
    std::vector<int> mag{3}; // Stored as X, Y, Z
    std::vector<int> lin_acceleration{3}; // Stored as X, Y, Z
    int time;
};