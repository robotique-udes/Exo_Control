#ifndef LOGIC_H
#define LOGIC_H
#include "dataCore.h"

class Logic 
{
private:

    DataCore& dataCore = DataCore::getInstance();
    // PID variables
    float ErrorCurrentRightKnee = 0.0;
    float ErrorCurrentLeftKnee = 0.0;
    float IntegralRightKnee = 0.0;
    float IntegralLeftKnee = 0.0;
    float DerivativeRightKnee = 0.0;
    float DerivativeLeftKnee = 0.0;
    float PreviousErrorRightKnee = 0.0;
    float PreviousErrorLeftKnee = 0.0;

    // Torques
    float LeftHipTorque = 0.0;
    float RightHipTorque = 0.0;
    float LeftKneeTorque = 0.0;
    float RightKneeTorque = 0.0;

    // Currents
    float RightKneeNeededCurrent = 0.0;
    float LeftKneeNeededCurrent = 0.0;
    float RightHipNeededCurrent = 0.0;
    float LeftHipNeededCurrent = 0.0;
    float RightKneeMeasuredCurrent = 0.0;
    float LeftKneeMeasuredCurrent = 0.0;
    float RightHipMeasuredCurrent = 0.0;
    float LeftHipMeasuredCurrent = 0.0;

public:
    void Update();
    void LimitMinMaxFloat(float &val, float max);
    void LimitMinMaxInt(int &val, int max);
    void neededTorque();
    void printTorque();
    void printPMW();
    void PIDCurrent();
    void neededCurrent();
    void PIDCurrentPrealable();
    float toDegrees(float radians);
    float toRadian(float degree);

};

#endif