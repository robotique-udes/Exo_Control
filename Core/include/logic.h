#ifndef LOGIC_H
#define LOGIC_H
#include "dataCore.h"

class Logic
{
private:
    DataCore &dataCore = DataCore::getInstance();

    // Torques
    float LeftHipTorque = 0.0;
    float RightHipTorque = 0.0;
    float LeftKneeTorque = 0.0;
    float RightKneeTorque = 0.0;

    // BATTERY ESTIMATION
    unsigned long previousTimeBatterie = 0;

    // Angles
    float LeftHipAngle = 0;
    float LeftKneeAngle = 0;
    float RightHipAngle = 0;
    float RightKneeAngle = 0;
    float LeftThighAngle = 0;
    float RightThighAngle = 0;
    float LeftTibiaAngle = 0;
    float RightTibiaAngle = 0;
    float ExoBackAngle = 0;

    // On ground
    bool LeftOnGround = false; // True if the left foot is on the ground
    bool RightOnGround = false; // True if the right foot is on the ground
    int NbOnGround = 0; // Number of feet on the ground

public:
    // -------------------------- TORQUE CALCULATION --------------------------
    /**
     * @brief Compute needed torque using either angles from encoders or IMUs
     */
    void neededTorque();

    /**
     * @brief Compute torque needed to keep the foot in the air
     * @param thighAngle Angle of the thigh
     * @param tibiaAngle Angle of the tibia
     * @param backAngle Angle of the back
     * @param isLeft True if the foot is the left one
     */
    void calculateTorqueFootInAir(float thighAngle, float tibiaAngle, float backAngle, bool isLeft);

    /**
     * @brief Compute torque needed counteract gravity when the foot is on the ground
     * @param thighAngle Angle of the thigh
     * @param tibiaAngle Angle of the tibia
     * @param backAngle Angle of the back
     * @param isLeft True if the foot is the left one
     */
    void calculateTorqueFootOnGround(float thighAngle, float tibiaAngle, float backAngle, bool isLeft);

    /**
     * @brief Check if the angles are within the limits
     */
    void checkAngleLimits();

    /**
     * @brief Limit the torque to the max value of the motor
     */
    void limitTorques();

    // -------------------------- GETTERS --------------------------
    /**
     * @brief Get angles from encoders or IMUs
     */
    void getAngles();

    /**
     * @brief Get the number of feet on the ground
     */
    void getOnGround();

    // -------------------------- BATTERY ESTIMATION --------------------------
    float totalEnergy = 0.0;
    void IntegralPowerConsumption();
    /**
     * @brief Compute torque and update required PWM by calling other functions
     */
    void Update();

    // -------------------------- UTILITIES --------------------------
    /**
     * @brief Cap input value to max/min value
     * @param val Value to map (float or int)
     * @param cap max/min reachable value (float or int)
     */
    template <typename T>
    void LimitMinMax(T &val, T cap);

    /**
     * @brief Print computed needed torque
     */
    void printTorque();

    /**
     * @brief Set all torques to 0
     */
    void resetTorque();

    /**
     * @brief Convert degree to radian equivalent
     * @param degrees Input degree value
     * @return Radian equivalent of the input degree
     */
    float toDegrees(float degrees);

    /**
     * @brief Convert radian to degree equivalent
     * @param radians Input radian value
     * @return Degree equivalent of the input radian
     */
    float toRadian(float radians);
};

#endif