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

    totalEnergy += totalpower * (float)dt * MILLIS_TO_HOUR; // W*milisecond to Wh
    previousTimeBatterie = time;
    Serial.print("  Power: ");
    Serial.print(totalpower);
    Serial.print("  Energy: ");
    Serial.print(totalEnergy);
}

void Logic::neededTorque()
{
    // Un torque positif est un torque qui force pour avoir un angle plus positif
    /*getAngles();
    getOnGround();

    if (NbOnGround > 0)
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
    }*/

    getAngles();

    //ignore the first frame because we need the derivative
    
    LeftKneeTorque = calculateKneeTorque(LeftThighAngle, LeftTibiaAngle);
    RightKneeTorque = calculateKneeTorque(RightThighAngle, RightTibiaAngle);
    LeftHipTorque = LeftKneeTorque + calculateHipTorque(LeftThighAngle, LeftTibiaAngle);
    RightHipTorque = RightKneeTorque + calculateHipTorque(RightThighAngle, RightTibiaAngle);


    //TODO reevaluate the changes 
    // previousTime = currentTime;
    // lastLeftHipAngle = LeftHipAngle;
    // lastLeftKneeAngle = LeftKneeAngle;
    // lastLeftHipAngle = LeftHipAngle;
    // lastLeftKneeAngle = LeftKneeAngle;

    //delay(1000);
}


/**
 * ref documentation a florent 
 * m1 - masse cuisse en kg
 * m2 - masse mollet en kg
 * a  - constante de proportionnalite (a trouver) - centre de masse de la cuisse 
 * b  - constante de proportionnalite (a trouver) - centre de masse du mollet
 * l1 - longueur cuisse en m
 * l2 - longueur mollet en m
 * teta1 - angle entre hanche et cuisse en rad
 * teta2 - angle entre cuisse et mollet en rad
 * g  - constante de gravite
 * */
float Logic::calculateHipTorque(float hipJoint, float kneeJoint)
{
    //TODO find better names for alpha and beta
    float alpha = calveMass * calveCenterMass *thighLength * calveLength;
    float beta = (thighCenterMass * thighMass + calveMass) * gravity * thighLength;
    return (beta * sin(hipJoint));

}

float Logic::calculateKneeTorque(float hipJoint, float kneeJoint)
{
    //TODO find better names for alpha and beta
    // float alpha = calveMass * calveCenterMass *thighLength * calveLength;
    // float beta = calveMass * gravity * calveCenterMass * calveLength;
    // float derivedAngles =  getDerivedTeta(kneeJoint, lastKneeJoint) * getDerivedTeta(hipJoint, lastHipJoint);
    
    return  (calveMass * gravity * calveCenterMass * calveLength * sin(kneeJoint));

}

void Logic::jambelEnLair(float hipJointRight, float hipJointLeft, float torsoAngle, float *rightKneeTorque, float *leftKneeTorque)
{
    float F1 = torsoMass * gravity * 
            ((abs(sin(hipJointLeft)) + (torsoLength/thighLength)*sin(torsoAngle))/(abs(sin(hipJointRight)) + abs(sin(hipJointLeft))));
    float F2 = torsoMass * gravity * 
            ((abs(sin(hipJointRight)) - (torsoLength/thighLength)*sin(torsoAngle))/(abs(sin(hipJointRight)) + abs(sin(hipJointLeft))));
    *rightKneeTorque = -1* (F1 + torsoCenterMass * thighMass * gravity) * thighLength * sin(hipJointRight);

    *leftKneeTorque = -1* (F2 + torsoCenterMass * thighMass * gravity) * thighLength * sin(hipJointLeft);


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
    LeftOnGround = dataCore.getLeftGrounded();
    RightOnGround = dataCore.getRightGrounded();
    NbOnGround = int(LeftOnGround) + int(RightOnGround);
}

void Logic::calculateTorqueFootInAir(float thighAngle, float tibiaAngle, float backAngle, bool isLeft)
{
    float hipTorqueFromThigh = (M_THIGH * G) * sin(thighAngle) * L_THIGH / 2;
    float hipTorqueFromTibia = (M_TIBIA * G) * (sin(thighAngle) * L_THIGH + sin(tibiaAngle) * L_TIBIA / 2);
    float hipTorque = hipTorqueFromThigh + hipTorqueFromTibia; // ok

    float kneeTorque = -(M_TIBIA * G) * sin(tibiaAngle) * L_TIBIA / 2; // ok

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
    float hipTorque = -(M_BACK * G) * sin(backAngle) * L_BACK / 2;
    hipTorque = hipTorque / float(NbOnGround); // On divise par le nombre de pieds au sol parce que le poids peut être supporté par les deux pieds

    float kneeTorqueFromThigh = -(M_THIGH * G) * sin(thighAngle) * L_THIGH / 2;
    float kneeTorqueFromBack = -(M_BACK * G) * (sin(backAngle) * L_BACK / 2 + sin(thighAngle) * L_THIGH);
    kneeTorqueFromBack = kneeTorqueFromBack / float(NbOnGround); // On divise par le nombre de pieds au sol parce que le poids peut être supporté par les deux pieds
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
    if (LeftKneeAngle > MAX_ANGLE_KNEE && LeftKneeTorque > 0 || LeftKneeAngle < MIN_ANGLE_KNEE && LeftKneeTorque < 0)
        LeftKneeTorque = 0;
    if (RightHipAngle > MAX_ANGLE_HIP && RightHipTorque > 0 || RightHipAngle < MIN_ANGLE_HIP && RightHipTorque < 0)
        RightHipTorque = 0;
    if (RightKneeAngle > MAX_ANGLE_KNEE && RightKneeTorque > 0 || RightKneeAngle < MIN_ANGLE_KNEE && RightKneeTorque < 0)
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
    Serial.print("  TorqueLeftHip: \t");
    Serial.print(LeftHipTorque);
    Serial.print("  TorqueRightHip: \t");
    Serial.print(RightHipTorque);
    Serial.print("  TorqueLeftKnee: \t");
    Serial.print(LeftKneeTorque);
    Serial.print("  TorqueRightKnee: \t");
    Serial.print(RightKneeTorque);

}

float Logic::toDegrees(float radians)
{
    return radians * 180 / PI;
}

float Logic::toRadian(float degree)
{
    return degree * PI / 180;
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