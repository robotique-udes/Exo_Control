#include "logic.h"

void Logic::Update()
{
    neededTorque();
}

// BATTERY ESTIMATION
void Logic::IntegralPowerConsumption()
{
    unsigned long time = millis();
    unsigned long dt = time - previousTimeBatterie;

    float leftHipPower = abs(LeftHipTorque) * MOTOR_W_PER_NM_HIP;
    float rightHipPower = abs(RightHipTorque) * MOTOR_W_PER_NM_HIP;
    float leftKneePower = abs(LeftKneeTorque) * MOTOR_W_PER_NM_KNEE;
    float rightKneePower = abs(RightKneeTorque) * MOTOR_W_PER_NM_KNEE;

    float totalpower = leftHipPower + rightHipPower + leftKneePower + rightKneePower;

    Serial.print("  Power: ");
    Serial.print(totalpower);
    totalEnergy += totalpower * (float)dt * MILLIS_TO_HOUR; // W*milisecond to Wh
    previousTimeBatterie = time;
}

template <typename T>
void Logic::LimitMinMax(T &val, T cap)
{
    if (std::is_same<T, int>::value || std::is_same<T, float>::value)
    {
        if (val > cap)
            val = cap;
        else if (val < -cap)
            val = -cap;
    }
    else
    {
        Serial.print("LimitMinMax Error - Invalid data type: ");
        Serial.println(val);
    }
}

void Logic::neededTorque()
{
    // Un torque positif est un torque qui force pour avoir un angle plus positif
    getAngles();
    getOnGround();

    if (dataCore.isMotorEnabled() || NbOnGround == 0)
    {
        if (RightOnGround)
            calculateTorqueFootOnGround(RightThighAngle, RightTibiaAngle, ExoBackAngle, false);
        else
            calculateTorqueFootInAir(RightThighAngle, RightTibiaAngle, ExoBackAngle, false);

        if (LeftOnGround)
            calculateTorqueFootOnGround(LeftThighAngle, LeftTibiaAngle, ExoBackAngle, true);
        else
            calculateTorqueFootInAir(LeftThighAngle, LeftTibiaAngle, ExoBackAngle, true);

        checkAngleLimits();
        limitTorques();
    }
    else
    {
        resetTorque();
    }
}

void Logic::getAngles()
{
    if (dataCore.getAngleSource() == FROM_IMU)
    {
        LeftHipAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::HIP_L));
        LeftKneeAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::KNEE_L));
        RightHipAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::HIP_R));
        RightKneeAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::KNEE_L));
        RightThighAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::THIGH_R));
        LeftThighAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::THIGH_L));
        RightTibiaAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::TIBIA_R));
        LeftTibiaAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::TIBIA_L));
        ExoBackAngle = toRadian(dataCore.getBnoAngles(EnumBnoAngle::EXO_BACK));
    }
    else
    {
        Serial.println("ERROR : ENCODER NOT SUPPORTED IN MATHS");
    }
}

void Logic::getOnGround()
{
    LeftOnGround = dataCore.getLeftProxi();
    RightOnGround = dataCore.getRightProxi();
    NbOnGround = int(LeftOnGround) + int(RightOnGround);
}

void Logic::calculateTorqueFootInAir(float thighAngle, float tibiaAngle, float backAngle, bool isLeft)
{
    // TODO : Test those equations to see if they are correct. I dont know if it's sin or cos that should be used
    float hipTorqueFromThigh = (M_THIGH * G) * sin(thighAngle) * L_THIGH / 2;
    float hipTorqueFromTibia = (M_TIBIA * G) * (sin(thighAngle) * L_THIGH + sin(tibiaAngle) * L_TIBIA / 2);
    float hipTorque = hipTorqueFromThigh + hipTorqueFromTibia;

    float kneeTorque = -(M_TIBIA * G) * sin(tibiaAngle) * L_TIBIA / 2;

    if (isLeft)
    {
        LeftHipTorque = hipTorque;
        LeftKneeTorque = kneeTorque;
    }
    else
    {
        RightHipTorque = hipTorque;
        RightKneeTorque = kneeTorque;
    }
}

void Logic::calculateTorqueFootOnGround(float thighAngle, float tibiaAngle, float backAngle, bool isLeft)
{
    // TODO : Test those equations to see if they are correct. I dont know if it's sin or cos that should be used

    float hipTorque = -(M_BACK * G) * cos(backAngle) * L_BACK / 2;
    hipTorque *= 1 / NbOnGround; // On divise par le nombre de pieds au sol parce que le poids peut être supporté par les deux pieds

    float kneeTorqueFromThigh = -(M_THIGH * G) * cos(thighAngle) * L_THIGH / 2;
    float kneeTorqueFromBack = -(M_BACK * G) * (cos(backAngle) * L_BACK / 2 + cos(thighAngle) * L_THIGH);
    kneeTorqueFromBack *= 1 / NbOnGround; // On divise par le nombre de pieds au sol parce que le poids peut être supporté par les deux pieds
    float kneeTorque = kneeTorqueFromThigh + kneeTorqueFromBack;

    if (isLeft)
    {
        LeftHipTorque = hipTorque;
        LeftKneeTorque = kneeTorque;
    }
    else
    {
        RightHipTorque = hipTorque;
        RightKneeTorque = kneeTorque;
    }
}

void Logic::checkAngleLimits()
{
    // Si on dépasse les angles max, on force le torque à 0
    if (LeftHipAngle > MAX_ANGLE_HIP && LeftHipTorque > 0 || LeftHipAngle < MIN_ANGLE_HIP && LeftHipTorque < 0)
        LeftHipTorque = 0;
    else if (LeftKneeAngle > MAX_ANGLE_KNEE && LeftKneeTorque > 0 || LeftKneeAngle < MIN_ANGLE_KNEE && LeftKneeTorque < 0)
        LeftKneeTorque = 0;
    else if (RightHipAngle > MAX_ANGLE_HIP && RightHipTorque > 0 || RightHipAngle < MIN_ANGLE_HIP && RightHipTorque < 0)
        RightHipTorque = 0;
    else if (RightKneeAngle > MAX_ANGLE_KNEE && RightKneeTorque > 0 || RightKneeAngle < MIN_ANGLE_KNEE && RightKneeTorque < 0)
        RightKneeTorque = 0;
}

void Logic::limitTorques()
{
    // Limit the torque to the max value of the motor. If the motor overheat, those values should be decreased
    LimitMinMax(LeftHipTorque, HIGHEST_HIP_TORQUE);
    LimitMinMax(RightHipTorque, HIGHEST_HIP_TORQUE);
    LimitMinMax(LeftKneeTorque, HIGHEST_KNEE_TORQUE);
    LimitMinMax(RightKneeTorque, HIGHEST_KNEE_TORQUE);
}

void Logic::resetTorque()
{
    LeftHipTorque = 0.0;
    RightHipTorque = 0.0;
    LeftKneeTorque = 0.0;
    RightKneeTorque = 0.0;
}

void Logic::printTorque()
{
    Serial.print("  NeededTorqueHL: \t");
    Serial.print(LeftHipTorque);
    Serial.print("  NeededTorqueRH: \t");
    Serial.print(RightHipTorque);
    Serial.print("  NeededTorqueLK: \t");
    Serial.print(LeftKneeTorque);
    Serial.print("  NeededTorqueRK: \t");
    Serial.println(RightKneeTorque);
}

float Logic::toDegrees(float radians)
{
    return radians * 180 / PI;
}

float Logic::toRadian(float degree)
{
    return degree * PI / 180;
}