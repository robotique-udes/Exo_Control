/**
 * @file Logic.cpp
 * @brief Implementation of the Logic class
 * 
 * @author Gabriel Desrochers
 * @date 2026-07-18
 */


#include "Logic.hpp"
#include "macros/printMacro.hpp"

static const bool DEBUG_PRINT = true; // true if you want to display prints in this file


Logic::Logic()
{
    m_morphologyMutex = xSemaphoreCreateMutex();
    setMorphology(170, 70);
}



void Logic::setMorphology(int p_height, int p_mass)
{
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) 
    {
        m_userHeight = p_height / 100.0; 
        m_userMass = p_mass;
        
        float gravitationalForce = m_userMass * exo_config::physics::GRAVITY;
        float exoForce = exo_config::physics::EXO_MASS * exo_config::physics::GRAVITY;

        m_lengthTorso = exo_config::anatomy::PROPORTION_TORSO_LENGTH * m_userHeight;   
        m_lengthThigh = exo_config::anatomy::PROPORTION_THIGH_LENGTH * m_userHeight;   
        m_lengthCalf = exo_config::anatomy::PROPORTION_CALF_LENGTH * m_userHeight;    

        m_forceTorso = exo_config::anatomy::PROPORTION_TORSO_MASS * gravitationalForce + exoForce; 
        m_forceThigh = exo_config::anatomy::PROPORTION_THIGH_MASS * gravitationalForce;   
        m_forceCalf = exo_config::anatomy::PROPORTION_CALF_MASS * gravitationalForce;  

        xSemaphoreGive(m_morphologyMutex);
    }
}


void Logic::calculateTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                            const bool p_grounded[exo_config::bnos::NB_LEG],
                            float (&p_torque)[exo_config::motors::AMOUNT])
{
    float torqueKneeL;
    float torqueHipL;
    float torqueKneeR;
    float torqueHipR;

    if (p_grounded[exo_config::bnos::LEFT_LEG] && p_grounded[exo_config::bnos::RIGHT_LEG])
    {
        float fnRight = 0.0;
        float fnLeft = 0.0;
        getNormalForces(p_angles, fnRight, fnLeft);
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH], fnLeft, 
                                torqueHipL, torqueKneeL);
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH], fnRight,
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[exo_config::bnos::LEFT_LEG])
    {
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH], 
                                m_forceTorso, torqueHipL, torqueKneeL);
        calculateTorqueAirborne(p_angles[exo_config::bnos::RIGHT_THIGH], p_angles[exo_config::bnos::RIGHT_SHIN], 
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[exo_config::bnos::RIGHT_LEG])
    {
        calculateTorqueGrounded(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH], 
                                m_forceTorso, torqueHipR, torqueKneeR);
        calculateTorqueAirborne(p_angles[exo_config::bnos::LEFT_THIGH], p_angles[exo_config::bnos::LEFT_SHIN], 
                                torqueHipL, torqueKneeL);
    }
    else 
    {
        //no torque if both foot of the ground (jumping)
        memset(p_torque, 0, sizeof(p_torque));
    }
    valideTorque(p_angles, p_torque);
    
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

void Logic::calculateTorqueAirborne(float p_angleHip, float p_angleKnee, float &p_torqueHip, float &p_torqueKnee)
{
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) 
    {
        float p_torqueKnee = m_forceCalf*m_lengthCalf/2.0*sin(radians(p_angleKnee));
        float p_torqueHip = p_torqueKnee + m_forceThigh*m_lengthThigh/2.0*sin(radians(p_angleHip))
                        + m_forceCalf*(m_lengthThigh*sin(radians(p_angleHip)) + m_lengthCalf/2.0*sin(radians(p_angleKnee)));
        xSemaphoreGive(m_morphologyMutex);
    }
}


void Logic::calculateTorqueGrounded(float p_angleTorso, float p_angleThigh, float p_forceOnLeg,
                                    float &p_torqueHip, float &p_torqueKnee)
{
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) 
    {
        p_torqueHip = m_lengthTorso/2.0*sin(radians(p_angleTorso)) * p_forceOnLeg;
        p_torqueKnee = -m_lengthThigh*sin(radians(p_angleThigh))*(0.5*m_forceThigh + p_forceOnLeg) + p_torqueHip;
        xSemaphoreGive(m_morphologyMutex);
    }
}


void Logic::getDistanceFromCenterMass(const float p_angles[exo_config::bnos::AMOUNT], 
                                      float& p_distLeftFoot, float& p_distRightFoot)
{
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) {
        p_distLeftFoot = m_lengthCalf*sin(radians(p_angles[exo_config::bnos::LEFT_SHIN]))
                        + m_lengthThigh*sin(radians(p_angles[exo_config::bnos::LEFT_THIGH]))
                        - m_lengthTorso/2.0*sin(radians(p_angles[exo_config::bnos::MOBO]));
        p_distRightFoot = m_lengthCalf*sin(radians(p_angles[exo_config::bnos::RIGHT_SHIN]))
                        + m_lengthThigh*sin(radians(p_angles[exo_config::bnos::RIGHT_THIGH]))
                        - m_lengthTorso/2.0*sin(radians(p_angles[exo_config::bnos::MOBO]));
        xSemaphoreGive(m_morphologyMutex);
    }    
}

void Logic::getNormalForces(const float p_angles[exo_config::bnos::AMOUNT], 
                            float& p_fnRight, float& p_fnLeft)
{
    float distLeftFoot;
    float distRightFoot;
    getDistanceFromCenterMass(p_angles, distLeftFoot, distRightFoot);

    float totalDist = abs(distLeftFoot - distRightFoot);
    
    if (totalDist < abs(distLeftFoot) || totalDist < abs(distRightFoot))
    {
        totalDist = 0.0;
    }
                    
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) {
        if (totalDist == 0)
        {
            p_fnRight = m_forceTorso/2.0;
            p_fnLeft = m_forceTorso/2.0;
        }
        else 
        {
            p_fnRight = m_forceTorso*distLeftFoot/totalDist;
            p_fnLeft = m_forceTorso*distRightFoot/totalDist;
        }

        xSemaphoreGive(m_morphologyMutex);
    }
}

void Logic::valideTorque(const float p_angles[exo_config::bnos::AMOUNT], float (&p_torque)[exo_config::motors::AMOUNT])
{  
    if (limitAngleHip(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH]))
    {
        PRINT("HIP LEFT not good : ");
        PRINTLN(p_angles[exo_config::bnos::MOBO] + p_angles[exo_config::bnos::LEFT_THIGH]);
        p_torque[exo_config::motors::HIP_LEFT] = 0.0;
    }
    if (limitAngleHip(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH]))
    {
        PRINT("HIP RIGHT not good : ");
        PRINTLN(p_angles[exo_config::bnos::MOBO] + p_angles[exo_config::bnos::RIGHT_THIGH]);
        p_torque[exo_config::motors::HIP_RIGHT] = 0.0;
    }
    if (limitAngleKnee(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::LEFT_THIGH], 
                        p_angles[exo_config::bnos::LEFT_SHIN]))
    {
        PRINT("KNEE LEFT not good : ");
        PRINTLN(p_angles[exo_config::bnos::MOBO] + p_angles[exo_config::bnos::LEFT_THIGH] 
                        + p_angles[exo_config::bnos::LEFT_SHIN]);
        p_torque[exo_config::motors::KNEE_LEFT] = 0.0;
    }
    if (limitAngleKnee(p_angles[exo_config::bnos::MOBO], p_angles[exo_config::bnos::RIGHT_THIGH], 
                        p_angles[exo_config::bnos::RIGHT_SHIN]))
    {
        PRINT("KNEE right not good : ");
        PRINTLN(p_angles[exo_config::bnos::MOBO] + p_angles[exo_config::bnos::RIGHT_THIGH] 
                        + p_angles[exo_config::bnos::RIGHT_SHIN]);
        p_torque[exo_config::motors::KNEE_RIGHT] = 0.0;
    }
}

bool Logic::limitAngleHip(float p_angleBack, float p_angleHip)
{
    const uint8_t max_angle_positive = 80;
    const int8_t max_angle_negative = -70;
    const uint8_t tolerence = 10;
    const float max_angle_hip[2] = {p_angleBack + max_angle_positive + tolerence, 
                                    p_angleBack + max_angle_negative - tolerence};
    if (p_angleHip > max_angle_hip[0])
        return true;
    if (p_angleHip < max_angle_hip[1])
        return true;
    else 
        return false;
}

bool Logic::limitAngleKnee(float p_angleBack, float p_angleHip, float p_angleKnee)
{
    const uint8_t max_angle_positive = 0;
    const int8_t max_angle_negative = -100;
    const uint8_t tolerence = 10;

    const float max_angle_hip[2] = {p_angleBack + p_angleHip + max_angle_positive + tolerence, 
                                    p_angleBack + p_angleHip + max_angle_negative - tolerence};

    if (p_angleKnee > max_angle_hip[0])
        return true;
    if (p_angleKnee < max_angle_hip[1])
        return true;
    else 
        return false;
}