#include "logic.h"


Logic::Logic()
{
    morphologyMutex = xSemaphoreCreateMutex();
    setMorphology(170, 70);
}



void Logic::setMorphology(int height, int mass)
{
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        userHeight = height / 100.0; 
        userMass = mass;
        
        float gravitationalForce = userMass * physics::gravity;
        float exoForce = physics::exo_mass * physics::gravity;

        lengthTorso = anatomy::proportion_torso_length * userHeight;   
        lengthThigh = anatomy::proportion_thigh_length * userHeight;   
        lengthCalf = anatomy::proportion_calf_length * userHeight;    

        forceTorso = anatomy::proportion_torso_mass * gravitationalForce + exoForce; 
        forceThigh = anatomy::proportion_thigh_mass * gravitationalForce;   
        forceCalf = anatomy::proportion_calf_mass * gravitationalForce;  

        xSemaphoreGive(morphologyMutex);
    }
}


void Logic::calculateTorque(const float angles[bno_config::amount], 
                            const bool grounded[bno_config::nb_leg],
                            float (&torque)[motor_config::amount])
{
    float torqueKneeL;
    float torqueHipL;
    float torqueKneeR;
    float torqueHipR;

    if (grounded[bno_config::left_leg] && grounded[bno_config::right_leg])
    {
        float fnRight = 0.0;
        float fnLeft = 0.0;
        getNormalForces(angles, fnRight, fnLeft);
        calculateTorqueGrounded(angles[bno_config::mobo], angles[bno_config::left_thigh], fnLeft, 
                                torqueHipL, torqueKneeL);
        calculateTorqueGrounded(angles[bno_config::mobo], angles[bno_config::right_thigh], fnRight,
                                torqueHipR, torqueKneeR);
    }
    else if (grounded[bno_config::left_leg])
    {
        calculateTorqueGrounded(angles[bno_config::mobo], angles[bno_config::left_thigh], 
                                forceTorso, torqueHipL, torqueKneeL);
        calculateTorqueAirborne(angles[bno_config::right_thigh], angles[bno_config::right_shin], 
                                grounded[bno_config::right_leg], torqueHipR, torqueKneeR);
    }
    else if (grounded[bno_config::right_leg])
    {
        calculateTorqueGrounded(angles[bno_config::mobo], angles[bno_config::right_thigh], 
                                forceTorso, torqueHipR, torqueKneeR);
        calculateTorqueAirborne(angles[bno_config::left_thigh], angles[bno_config::left_shin], 
                                grounded[bno_config::left_leg], torqueHipL, torqueKneeL);
    }
    else 
    {
        //no torque if both foot of the ground (jumping)
        memset(torque, 0, sizeof(torque));
    }
    valideTorque(angles, torque);
    
    torque[motor_config::hip_left] = torqueHipL;
    torque[motor_config::hip_right] = torqueHipR;
    torque[motor_config::knee_left] = torqueKneeL;
    torque[motor_config::knee_right] = torqueKneeR;
    
    if (debug::logic)
    {
        Serial.print("Torque Knee Right ");
        Serial.println(torque[motor_config::knee_right]);
        Serial.print("Torque Hip Right ");
        Serial.println(torque[motor_config::hip_right]);
        Serial.print("Torque Knee Left ");
        Serial.println(torque[motor_config::knee_left]);
        Serial.print("Torque Hip Left ");
        Serial.println(torque[motor_config::hip_left]);
        Serial.println("-------------------");
        Serial.println();
    }

}

void Logic::calculateTorqueAirborne(float angleHip, float angleKnee, bool grounded,
                                        float &torqueHip, float &torqueKnee)
{
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        float torqueKnee = forceCalf*lengthCalf/2.0*sin(radians(angleKnee));
        float torqueHip = torqueKnee + forceThigh*lengthThigh/2.0*sin(radians(angleHip))
                        + forceCalf*(lengthThigh*sin(radians(angleHip)) + lengthCalf/2.0*sin(radians(angleKnee)));
        xSemaphoreGive(morphologyMutex);
    }
}


void Logic::calculateTorqueGrounded(float angleTorso, float angleThigh, float forceOnLeg,
                                    float &torqueHip, float &torqueKnee)
{
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        torqueHip = lengthTorso/2.0*sin(radians(angleTorso)) * forceOnLeg;
        torqueKnee = -lengthThigh*sin(radians(angleThigh))*(0.5*forceThigh + forceOnLeg) + torqueHip;
        xSemaphoreGive(morphologyMutex);
    }
/*     Serial.print("torque hip ");
    Serial.println(torqueHip);
    Serial.print("lengthTorso ");
    Serial.println(lengthTorso);
    Serial.print("angleTorso ");
    Serial.println(angleTorso);
    Serial.print("forceOnLeg ");
    Serial.println(forceOnLeg); */
}


void Logic::getDistanceFromCenterMass(const float angles[bno_config::amount], 
                                      float& distLeftFoot, float& distRightFoot)
{
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        distLeftFoot = lengthCalf*sin(radians(angles[bno_config::left_shin]))
                        + lengthThigh*sin(radians(angles[bno_config::left_thigh]))
                        - lengthTorso/2.0*sin(radians(angles[bno_config::mobo]));
        distRightFoot = lengthCalf*sin(radians(angles[bno_config::right_shin]))
                        + lengthThigh*sin(radians(angles[bno_config::right_thigh]))
                        - lengthTorso/2.0*sin(radians(angles[bno_config::mobo]));
        xSemaphoreGive(morphologyMutex);
    }    
}


void Logic::getNormalForces(const float angles[bno_config::amount], 
                            float& fnRight, float& fnLeft)
{
    float distLeftFoot;
    float distRightFoot;
    getDistanceFromCenterMass(angles, distLeftFoot, distRightFoot);

    float totalDist = abs(distLeftFoot - distRightFoot);
    
    if (totalDist < abs(distLeftFoot) || totalDist < abs(distRightFoot))
    {
        //TODO crab mode
        totalDist = 0.0;
    }
                    
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        if (totalDist == 0)
        {
            fnRight = forceTorso/2.0;
            fnLeft = forceTorso/2.0;
        }
        else 
        {
            fnRight = forceTorso*distLeftFoot/totalDist;
            fnLeft = forceTorso*distRightFoot/totalDist;
        }

        xSemaphoreGive(morphologyMutex);
    }
}


void Logic::valideTorque(const float angles[bno_config::amount], float (&torque)[motor_config::amount])
{  
    if (limitAngleHip(angles[bno_config::mobo], angles[bno_config::left_thigh]))
    {
        if (debug::logic)
        {
            Serial.print("HIP LEFT not good : ");
            Serial.println(angles[bno_config::mobo] + angles[bno_config::left_thigh]);
        }
        torque[motor_config::hip_left] = 0.0;
    }
    if (limitAngleHip(angles[bno_config::mobo], angles[bno_config::right_thigh]))
    {
        if (debug::logic)
        {   
            Serial.print("HIP RIGHT not good : ");
            Serial.println(angles[bno_config::mobo] + angles[bno_config::right_thigh]);
        }
        torque[motor_config::hip_right] = 0.0;
    }
    if (limitAngleKnee(angles[bno_config::mobo], angles[bno_config::left_thigh], 
                        angles[bno_config::left_shin]))
    {
        if (debug::logic)
        {
            Serial.print("KNEE LEFT not good : ");
            Serial.println(angles[bno_config::mobo] + angles[bno_config::left_thigh] 
                            + angles[bno_config::left_shin]);
        }
        torque[motor_config::knee_left] = 0.0;
    }
    if (limitAngleKnee(angles[bno_config::mobo], angles[bno_config::right_thigh], 
                        angles[bno_config::right_shin]))
    {
        if (debug::logic)
        {
            Serial.print("KNEE right not good : ");
            Serial.println(angles[bno_config::mobo] + angles[bno_config::right_thigh] 
                            + angles[bno_config::right_shin]);
        }
        torque[motor_config::knee_right] = 0.0;
    }
}


bool Logic::limitAngleHip(float angleBack, float angleHip)
{
    const uint8_t max_angle_positive = 80;
    const int8_t max_angle_negative = -70;
    const uint8_t tolerence = 10;
    const float max_angle_hip[2] = {angleBack + max_angle_positive + tolerence, 
                                    angleBack + max_angle_negative - tolerence};
    if (angleHip > max_angle_hip[0])
        return true;
    if (angleHip < max_angle_hip[1])
        return true;
    else 
        return false;
}

bool Logic::limitAngleKnee(float angleBack, float angleHip, float angleKnee)
{
    const uint8_t max_angle_positive = 0;
    const int8_t max_angle_negative = -100;
    const uint8_t tolerence = 10;

    const float max_angle_hip[2] = {angleBack + angleHip + max_angle_positive + tolerence, 
                                    angleBack + angleHip + max_angle_negative - tolerence};

    if (angleKnee > max_angle_hip[0])
        return true;
    if (angleKnee < max_angle_hip[1])
        return true;
    else 
        return false;
}