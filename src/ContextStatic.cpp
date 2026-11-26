/**
 * @file ContextStatic.cpp
 * @brief Implementation of the ContextStatic class
 * 
 * @author Gabriel Desrochers, Éloi Charbonneau
 * @date 2026-09-26
 */

#include "ContextStatic.hpp"
#include "macros/printMacro.hpp"

static const bool DEBUG_PRINT = true; // true if you want to display prints in this file

static constexpr uint8_t MONITORED_ANGLES[] = {
    exo_config::bnos::LEFT_THIGH, exo_config::bnos::RIGHT_THIGH,
    exo_config::bnos::LEFT_SHIN, exo_config::bnos::RIGHT_SHIN};

ContextStatic::ContextStatic()
{
    for (uint8_t bno = 0; bno < exo_config::bnos::AMOUNT; ++bno)
    {
        m_deltaAverages[bno].setPeriod(exo_config::context::ANGLE_DELTA_WINDOW);
    }
}

float ContextStatic::computeContextValidity(const float p_angles[exo_config::bnos::AMOUNT])
{
    float minValidity = 1.0f;

    for (const uint8_t bno : MONITORED_ANGLES)
    {
        const float angle = p_angles[bno];
        const float delta = m_hasPreviousAngles ? fabsf(angle - m_previousAngles[bno]) : 0.0f;
        m_previousAngles[bno] = angle;

        const float averageDelta = m_deltaAverages[bno].addValue(delta);
        const float validity = constrain(1.0f - averageDelta, 0.0f, 1.0f);
        minValidity = fminf(minValidity, validity);
    }

    m_hasPreviousAngles = true;
    return minValidity;
}

void ContextStatic::computeTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                            const bool p_grounded[exo_config::bnos::NB_LEG],
                            float (&p_torque)[exo_config::motors::AMOUNT])
{
    float torqueKneeL = 0.0f;
    float torqueHipL = 0.0f;
    float torqueKneeR = 0.0f;
    float torqueHipR = 0.0f;

    if (p_grounded[exo_config::bnos::LEFT_LEG] && p_grounded[exo_config::bnos::RIGHT_LEG])
    {
        float fnRight = 0.0f;
        float fnLeft = 0.0f;
        getNormalForces(p_angles, fnRight, fnLeft);
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH], fnLeft, 
                                torqueHipL, torqueKneeL);
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH], fnRight,
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[exo_config::bnos::LEFT_LEG])
    {
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH], 
                                m_morphology.getForceTorso(), torqueHipL, torqueKneeL);
        calculateTorqueAirborne(p_angles[exo_config::bnos::RIGHT_THIGH], p_angles[exo_config::bnos::RIGHT_SHIN], 
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[exo_config::bnos::RIGHT_LEG])
    {
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH], 
                                m_morphology.getForceTorso(), torqueHipR, torqueKneeR);
        calculateTorqueAirborne(p_angles[exo_config::bnos::LEFT_THIGH], p_angles[exo_config::bnos::LEFT_SHIN], 
                                torqueHipL, torqueKneeL);
    }

    validateTorque(p_angles, p_torque);
    
    p_torque[exo_config::motors::HIP_LEFT] = torqueHipL;
    p_torque[exo_config::motors::HIP_RIGHT] = torqueHipR;
    p_torque[exo_config::motors::KNEE_LEFT] = torqueKneeL;
    p_torque[exo_config::motors::KNEE_RIGHT] = torqueKneeR;
    
    PRINT("Torque Knee Right ");
    PRINTLN(p_torque[exo_config::motors::KNEE_RIGHT]);
    PRINT("Torque Hip Right ");
    PRINTLN(p_torque[exo_config::motors::HIP_RIGHT]);
    PRINT("Torque Knee Left ");
    PRINTLN(p_torque[exo_config::motors::KNEE_LEFT]);
    PRINT("Torque Hip Left ");
    PRINTLN(p_torque[exo_config::motors::HIP_LEFT]);
    PRINTLN("-------------------");
    PRINTLN();
}

void ContextStatic::calculateTorqueAirborne(float p_angleHip, float p_angleKnee, float &p_torqueHip, float &p_torqueKnee)
{
    const float lengthThigh = m_morphology.getLengthThigh();
    const float lengthCalf = m_morphology.getLengthCalf();
    const float forceThigh = m_morphology.getForceThigh();
    const float forceCalf = m_morphology.getForceCalf();

    p_torqueKnee = forceCalf*lengthCalf/2.0*sin(radians(p_angleKnee));
    p_torqueHip = p_torqueKnee + forceThigh*lengthThigh/2.0*sin(radians(p_angleHip))
                    + forceCalf*(lengthThigh*sin(radians(p_angleHip)) + lengthCalf/2.0*sin(radians(p_angleKnee)));
}

void ContextStatic::calculateTorqueGrounded(float p_angleTorso, float p_angleThigh, float p_forceOnLeg,
                                            float &p_torqueHip, float &p_torqueKnee)
{
    const float lengthTorso = m_morphology.getLengthTorso();
    const float lengthThigh = m_morphology.getLengthThigh();
    const float forceThigh = m_morphology.getForceThigh();

    p_torqueHip = lengthTorso/2.0*sin(radians(p_angleTorso)) * p_forceOnLeg;
    p_torqueKnee = -lengthThigh*sin(radians(p_angleThigh))*(0.5*forceThigh + p_forceOnLeg) + p_torqueHip;
}

float ContextStatic::calculateDistanceFromCenterMass(float p_angleTorso, float p_angleThigh, float p_angleShin)
{
    return m_morphology.getLengthCalf()*sin(radians(p_angleShin))
            + m_morphology.getLengthThigh()*sin(radians(p_angleThigh))
            - m_morphology.getLengthTorso()/2.0*sin(radians(p_angleTorso));
}

void ContextStatic::getDistanceFromCenterMass(const float p_angles[exo_config::bnos::AMOUNT], 
                                              float& p_distLeftFoot, float& p_distRightFoot)
{
    p_distLeftFoot = calculateDistanceFromCenterMass(p_angles[exo_config::bnos::MOBO], 
                                                     p_angles[exo_config::bnos::LEFT_THIGH], 
                                                     p_angles[exo_config::bnos::LEFT_SHIN]);
    p_distRightFoot = calculateDistanceFromCenterMass(p_angles[exo_config::bnos::MOBO], 
                                                      p_angles[exo_config::bnos::RIGHT_THIGH], 
                                                      p_angles[exo_config::bnos::RIGHT_SHIN]);
}

void ContextStatic::getNormalForces(const float p_angles[exo_config::bnos::AMOUNT], 
                                    float& p_fnRight, float& p_fnLeft)
{
    float distLeftFoot;
    float distRightFoot;
    getDistanceFromCenterMass(p_angles, distLeftFoot, distRightFoot);

    float totalDist = fabsf(distLeftFoot - distRightFoot);
    
    if (totalDist < fabsf(distLeftFoot) || totalDist < fabsf(distRightFoot))
    {
        totalDist = 0.0f;
    }

    const float forceTorso = m_morphology.getForceTorso();

    if (totalDist == 0)
    {
        p_fnRight = forceTorso/2.0;
        p_fnLeft = forceTorso/2.0;
    }
    else 
    {
        p_fnRight = forceTorso*distLeftFoot/totalDist;
        p_fnLeft = forceTorso*distRightFoot/totalDist;
    }
}

void ContextStatic::validateTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                                   float (&p_torque)[exo_config::motors::AMOUNT])
{
    const char* legNames[exo_config::bnos::NB_LEG] = {"LEFT", "RIGHT"};
    const uint8_t thighSensors[exo_config::bnos::NB_LEG] = {exo_config::bnos::LEFT_THIGH, 
                                                            exo_config::bnos::RIGHT_THIGH};
    const uint8_t shinSensors[exo_config::bnos::NB_LEG] = {exo_config::bnos::LEFT_SHIN, 
                                                           exo_config::bnos::RIGHT_SHIN};
    const uint8_t hipMotors[exo_config::bnos::NB_LEG] = {exo_config::motors::HIP_LEFT, 
                                                         exo_config::motors::HIP_RIGHT};
    const uint8_t kneeMotors[exo_config::bnos::NB_LEG] = {exo_config::motors::KNEE_LEFT, 
                                                          exo_config::motors::KNEE_RIGHT};
    const float angleBack = p_angles[exo_config::bnos::MOBO];

    for (uint8_t leg = 0; leg < exo_config::bnos::NB_LEG; ++leg)
    {
        const float angleThigh = p_angles[thighSensors[leg]];
        const float angleShin = p_angles[shinSensors[leg]];

        if (isHipAngleOutOfLimit(angleBack, angleThigh))
        {
            PRINT("HIP ");
            PRINT(legNames[leg]);
            PRINT(" not good : ");
            PRINTLN(angleBack + angleThigh);
            p_torque[hipMotors[leg]] = 0.0f;
        }
        if (isKneeAngleOutOfLimit(angleBack, angleThigh, angleShin))
        {
            PRINT("KNEE ");
            PRINT(legNames[leg]);
            PRINT(" not good : ");
            PRINTLN(angleBack + angleThigh + angleShin);
            p_torque[kneeMotors[leg]] = 0.0f;
        }
    }
}

bool ContextStatic::isHipAngleOutOfLimit(float p_angleBack, float p_angleHip)
{
    const float maxAngle = p_angleBack + exo_config::anatomy::MAX_HIP_ANGLE 
                            + exo_config::anatomy::ANGLE_LIMIT_TOLERANCE;
    const float minAngle = p_angleBack + exo_config::anatomy::MIN_HIP_ANGLE 
                            - exo_config::anatomy::ANGLE_LIMIT_TOLERANCE;

    return p_angleHip > maxAngle || p_angleHip < minAngle;
}

bool ContextStatic::isKneeAngleOutOfLimit(float p_angleBack, float p_angleHip, float p_angleKnee)
{
    const float maxAngle = p_angleBack + p_angleHip + exo_config::anatomy::MAX_KNEE_ANGLE 
                            + exo_config::anatomy::ANGLE_LIMIT_TOLERANCE;
    const float minAngle = p_angleBack + p_angleHip + exo_config::anatomy::MIN_KNEE_ANGLE 
                            - exo_config::anatomy::ANGLE_LIMIT_TOLERANCE;

    return p_angleKnee > maxAngle || p_angleKnee < minAngle;
}
