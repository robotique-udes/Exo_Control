#pragma once
#include <Arduino.h>

class BluetoothDataInterpreter
{
private:
    enum DataType {
        MOTOR_STATE = 1,
        HEIGHT = 2,
        WEIGHT = 3,
    };

    struct Data
    {
        bool stopMotors;
        int height;
        int weight;
        int test;
        char charTest;
    };

    Data data;

public:
    BluetoothDataInterpreter();
    ~BluetoothDataInterpreter();

    void interpretData(String rawString);

    bool getMotorState();
    int getHeight();
    int getWeight();
    int getTest();
    int getCharTest();

};