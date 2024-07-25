#ifndef LOGIC_H
#define LOGIC_H
#include "dataCore.h"

class Logic 
{
private:

    DataCore& dataCore = DataCore::getInstance();
    // PID variables (Unused at the moment)
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

    // Currents (Unused at the moment)
    float RightKneeNeededCurrent = 0.0;
    float LeftKneeNeededCurrent = 0.0;
    float RightHipNeededCurrent = 0.0;
    float LeftHipNeededCurrent = 0.0;
    float RightKneeMeasuredCurrent = 0.0;
    float LeftKneeMeasuredCurrent = 0.0;
    float RightHipMeasuredCurrent = 0.0;
    float LeftHipMeasuredCurrent = 0.0;

public:
    /**
     * @brief Compute torque and update required PWM by calling other functions
     */
    void Update();

    /**
     * @brief Cap input value to max/min value
     * @param val Value tu map (float or int)
     * @param max max/min reachable value (float or int)
     */
    template <typename T> void LimitMinMax(T &val, T cap);

    /**
     * @brief Compute needed torque using either angles from encoders or IMUs
     */
    void neededTorque();

    /**
     * @brief Print computed needed torque
     */
    void printTorque();

    /**
     * @brief Print computed needed PWM
     */
    void printPMW();

    /**
     * @brief Compute PID on needed current ---UNUSED---
     */
    void PIDCurrent();

    /**
     * @brief Compute needed current ---UNUSED---
     */
    void neededCurrent();

    /**
     * @brief Map torque to PWM and update value in dataCore
     */
    void PIDCurrentPrealable();

    /**
     * @brief Convert degree in radian equivalent 
     * @param radians Input radian value
     */
    float toDegrees(float radians);

    /**
     * @brief Convert radian in degree equivalent 
     * @param radians Input degree value
     */
    float toRadian(float degree);

};

#endif