#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include "enum.h"
#include "sonar.h"
#include "define.h"
#include <PinExtender.h>

class Motor
{
private:

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

    // IMU angles
    float RightHipAngle = 0.0;
    float RightKneeAngle = 0.0;
    float LeftHipAngle = 0.0;
    float LeftKneeAngle = 0.0;

    // Currents
    float RightKneeNeededCurrent = 0.0;
    float LeftKneeNeededCurrent = 0.0;
    float RightHipNeededCurrent = 0.0;
    float LeftHipNeededCurrent = 0.0;
    float RightKneeMeasuredCurrent = 0.0;
    float LeftKneeMeasuredCurrent = 0.0;
    float RightHipMeasuredCurrent = 0.0;
    float LeftHipMeasuredCurrent = 0.0;

    // control sonar
    Sonar sonar;

public:
    Motor();
    ~Motor();

    int PWMRightKnee = 0;
    int PWMLeftKnee = 0;
    int PWMRightHip = 0;
    int PWMLeftHip = 0;

    void sonarRead();
    void setPins();
    void readCurrent();
    void LimitMinMaxFloat(float &val, float max);
    void LimitMinMaxInt(int &val, int max);
    void neededTorque();
    void neededCurrent();
    void PIDCurrent();
    void PIDCurrentPrealable();
    void sendCommand();
    void setMotorMode(bool state);
    void setPower(double p);
    double getPower();

    void setAngle(enumIMU enumType, float val);
    float toDegrees(float radians);
    void printSonar();
    void printTorque();

    void setSonarState(bool state);
    void setHeight(double h);
    double sonarScanR();
    double sonarScanL();

    void motorSetSpeed(int ID, int val);

    void testFuncSetPwnHigh();
};

#endif