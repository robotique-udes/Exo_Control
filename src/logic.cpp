#include "logic.hpp"


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
        
        float gravitationalForce = m_userMass * physics::gravity;
        float exoForce = physics::exo_mass * physics::gravity;

        m_lengthTorso = anatomy::proportion_torso_length * m_userHeight;   
        m_lengthThigh = anatomy::proportion_thigh_length * m_userHeight;   
        m_lengthCalf = anatomy::proportion_calf_length * m_userHeight;    

        m_forceTorso = anatomy::proportion_torso_mass * gravitationalForce + exoForce; 
        m_forceThigh = anatomy::proportion_thigh_mass * gravitationalForce;   
        m_forceCalf = anatomy::proportion_calf_mass * gravitationalForce;  

        xSemaphoreGive(m_morphologyMutex);
    }
}


void Logic::calculateTorque(const float p_angles[bno_config::amount], 
                            const bool p_grounded[bno_config::nb_leg],
                            float (&p_torque)[motor_config::amount])
{
    float torqueKneeL;
    float torqueHipL;
    float torqueKneeR;
    float torqueHipR;

    if (p_grounded[bno_config::left_leg] && p_grounded[bno_config::right_leg])
    {
        float fnRight = 0.0;
        float fnLeft = 0.0;
        getNormalForces(p_angles, fnRight, fnLeft);
        calculateTorqueGrounded(p_angles[bno_config::mobo], p_angles[bno_config::left_thigh], fnLeft, 
                                torqueHipL, torqueKneeL);
        calculateTorqueGrounded(p_angles[bno_config::mobo], p_angles[bno_config::right_thigh], fnRight,
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[bno_config::left_leg])
    {
        calculateTorqueGrounded(p_angles[bno_config::mobo], p_angles[bno_config::left_thigh], 
                                m_forceTorso, torqueHipL, torqueKneeL);
        calculateTorqueAirborne(p_angles[bno_config::right_thigh], p_angles[bno_config::right_shin], 
                                torqueHipR, torqueKneeR);
    }
    else if (p_grounded[bno_config::right_leg])
    {
        calculateTorqueGrounded(p_angles[bno_config::mobo], p_angles[bno_config::right_thigh], 
                                m_forceTorso, torqueHipR, torqueKneeR);
        calculateTorqueAirborne(p_angles[bno_config::left_thigh], p_angles[bno_config::left_shin], 
                                torqueHipL, torqueKneeL);
    }
    else 
    {
        //no torque if both foot of the ground (jumping)
        memset(p_torque, 0, sizeof(p_torque));
    }
    valideTorque(p_angles, p_torque);
    
    p_torque[motor_config::hip_left] = torqueHipL;
    p_torque[motor_config::hip_right] = torqueHipR;
    p_torque[motor_config::knee_left] = torqueKneeL;
    p_torque[motor_config::knee_right] = torqueKneeR;
    
    if (debug::logic)
    {
        Serial.print("Torque Knee Right ");
        Serial.println(p_torque[motor_config::knee_right]);
        Serial.print("Torque Hip Right ");
        Serial.println(p_torque[motor_config::hip_right]);
        Serial.print("Torque Knee Left ");
        Serial.println(p_torque[motor_config::knee_left]);
        Serial.print("Torque Hip Left ");
        Serial.println(p_torque[motor_config::hip_left]);
        Serial.println("-------------------");
        Serial.println();
    }

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


void Logic::getDistanceFromCenterMass(const float p_angles[bno_config::amount], 
                                      float& p_distLeftFoot, float& p_distRightFoot)
{
    if (xSemaphoreTake(m_morphologyMutex, portMAX_DELAY) == pdTRUE) {
        p_distLeftFoot = m_lengthCalf*sin(radians(p_angles[bno_config::left_shin]))
                        + m_lengthThigh*sin(radians(p_angles[bno_config::left_thigh]))
                        - m_lengthTorso/2.0*sin(radians(p_angles[bno_config::mobo]));
        p_distRightFoot = m_lengthCalf*sin(radians(p_angles[bno_config::right_shin]))
                        + m_lengthThigh*sin(radians(p_angles[bno_config::right_thigh]))
                        - m_lengthTorso/2.0*sin(radians(p_angles[bno_config::mobo]));
        xSemaphoreGive(m_morphologyMutex);
    }    
}

void Logic::getNormalForces(const float p_angles[bno_config::amount], 
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

void Logic::valideTorque(const float p_angles[bno_config::amount], float (&p_torque)[motor_config::amount])
{  
    if (limitAngleHip(p_angles[bno_config::mobo], p_angles[bno_config::left_thigh]))
    {
        if (debug::logic)
        {
            Serial.print("HIP LEFT not good : ");
            Serial.println(p_angles[bno_config::mobo] + p_angles[bno_config::left_thigh]);
        }
        p_torque[motor_config::hip_left] = 0.0;
    }
    if (limitAngleHip(p_angles[bno_config::mobo], p_angles[bno_config::right_thigh]))
    {
        if (debug::logic)
        {   
            Serial.print("HIP RIGHT not good : ");
            Serial.println(p_angles[bno_config::mobo] + p_angles[bno_config::right_thigh]);
        }
        p_torque[motor_config::hip_right] = 0.0;
    }
    if (limitAngleKnee(p_angles[bno_config::mobo], p_angles[bno_config::left_thigh], 
                        p_angles[bno_config::left_shin]))
    {
        if (debug::logic)
        {
            Serial.print("KNEE LEFT not good : ");
            Serial.println(p_angles[bno_config::mobo] + p_angles[bno_config::left_thigh] 
                            + p_angles[bno_config::left_shin]);
        }
        p_torque[motor_config::knee_left] = 0.0;
    }
    if (limitAngleKnee(p_angles[bno_config::mobo], p_angles[bno_config::right_thigh], 
                        p_angles[bno_config::right_shin]))
    {
        if (debug::logic)
        {
            Serial.print("KNEE right not good : ");
            Serial.println(p_angles[bno_config::mobo] + p_angles[bno_config::right_thigh] 
                            + p_angles[bno_config::right_shin]);
        }
        p_torque[motor_config::knee_right] = 0.0;
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