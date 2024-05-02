#include <vector>

struct BNOStruct
{
    quat:               std::vector<int> v(4); // Stored as W, X, Y, Z
    euler:              std::vector<int> v(3); // Stored as yaw, pitch, roll as radians
    acceleration:       std::vector<int> v(3); // Stored as X, Y, Z
    gyro:               std::vector<int> v(3); // Stored as X, Y, Z
    mag:                std::vector<int> v(3); // Stored as X, Y, Z
    lin_acceleration:   std::vector<int> v(3); // Stored as X, Y, Z
    time:               int;
};
