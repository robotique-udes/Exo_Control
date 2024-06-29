#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <Arduino.h>
#include "enums.h"
#include "define.h"
#include "PinExtender.h"
#include "dataCore.h"
#include "ProxiSensor.h"
#include "multiplex.h"

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

    // proximity sensor
    Multiplex mux;
    ProxiSensor *LeftProxim;
    ProxiSensor *RightProxim;
    //settings
    DataCore& settings = DataCore::getInstance();

public:
    Motor();
    ~Motor();

    int PWMRightKnee = 0;
    int PWMLeftKnee = 0;
    int PWMRightHip = 0;
    int PWMLeftHip = 0;

    void setPins();
    void readCurrent();
    void LimitMinMaxFloat(float &val, float max);
    void LimitMinMaxInt(int &val, int max);
    void neededTorque();
    void neededCurrent();
    void PIDCurrent();
    void PIDCurrentPrealable();
    void sendCommand();
    void setPower(double p);
    double getPower();
    void SetTriggerDistance();

    void setAngle(enumIMU enumType, float val);
    float getAngle(enumIMU enumType);
    float toDegrees(float radians);
    float toRadian(float degree);
    void printProxim();
    void printTorque();
    void printPMW();

    void setSonarState(bool state);
    void setHeight(double h);

    void motorSetSpeed(int ID, int val); // ID 0 to 3, val -4096 to 4096

    void testFuncSetPwnHigh();
};

#endif