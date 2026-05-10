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
        
        float gravitationalForce = userMass*GRAVITY;
        float exoForce = EXO_MASS*GRAVITY;

        lengthTorso = 0.47*userHeight;   
        lengthThigh = 0.245*userHeight;   
        lengthCalf = 0.285*userHeight;    

        forceTorso = 0.678*gravitationalForce + exoForce; 
        forceThigh = 0.1*gravitationalForce;   
        forceCalf = 0.061*gravitationalForce;  

        xSemaphoreGive(morphologyMutex);
    }
}


void Logic::calculateTorque(RequiredData data, float (&torque)[NB_MOTOR])
{
  
   if (data.groundedL && data.groundedR)
    {
        float fnRight = 0.0;
        float fnLeft = 0.0;
        getNormalForces(data, fnRight, fnLeft);
        calculateTorqueGrounded(data.backAngle,data.hipAngleL, fnLeft, torque);
        calculateTorqueGrounded(data.backAngle,data.hipAngleR, fnRight, torque+2);
    }
    else if (data.groundedL)
    {
        calculateTorqueGrounded(data.backAngle,data.hipAngleL, forceTorso, torque);
        calculateTorqueAirborne(data.hipAngleR,data.kneeAngleR, data.groundedR, torque+2);
    }
    else if (data.groundedR)
    {
        calculateTorqueGrounded(data.backAngle,data.hipAngleR, forceTorso, torque+2);
        calculateTorqueAirborne(data.hipAngleL,data.kneeAngleL, data.groundedL, torque);
    }
    else 
    {
        //no torque if both foot of the ground (jumping)
        memset(torque, 0, sizeof(torque));
    }
    valideTorque(data, torque);
    
    Serial.print("Torque Knee Right ");
    Serial.println(torque[2]);
    Serial.print("Torque Hip Right ");
    Serial.println(torque[3]);
    Serial.print("Torque Knee Left ");
    Serial.println(torque[0]);
    Serial.print("Torque Hip Left ");
    Serial.println(torque[1]);
    Serial.println("-------------------");
    Serial.println();
}

void Logic::calculateTorqueAirborne(float angleHip, float angleKnee, bool grounded, float torque[2])
{
    float torqueKnee;
    float torqueHip;
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        float torqueKnee = forceCalf*lengthCalf/2.0*sin(radians(angleKnee));
        float torqueHip = torqueKnee + forceThigh*lengthThigh/2.0*sin(radians(angleHip))
                        + forceCalf*(lengthThigh*sin(radians(angleHip)) + lengthCalf/2.0*sin(radians(angleKnee)));
        xSemaphoreGive(morphologyMutex);
    }
    
    limitMinMax(torqueKnee, MAX_TORQUE);
    limitMinMax(torqueHip, MAX_TORQUE);
    torque[0] = torqueKnee;
    torque[1] = torqueHip;
}


void Logic::calculateTorqueGrounded(float angleTorso, float angleThigh, float forceOnLeg, float torque[2])
{
    float torqueKnee = 0.0;
    float torqueHip = 0.0;
    Serial.println(angleThigh);
    Serial.println(forceOnLeg);
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        torqueHip = lengthTorso/2.0*sin(radians(angleTorso)) * forceOnLeg;
        torqueKnee = -lengthThigh*sin(radians(angleThigh))*(0.5*forceThigh + forceOnLeg) + torqueHip;
        xSemaphoreGive(morphologyMutex);
    }
    limitMinMax(torqueKnee, MAX_TORQUE);
    limitMinMax(torqueHip, MAX_TORQUE);
    torque[0] = torqueKnee;
    torque[1] = torqueHip;
}


void Logic::getDistanceFromCenterMass(RequiredData data, float& distLeftFoot, float& distRightFoot)
{
    if (xSemaphoreTake(morphologyMutex, portMAX_DELAY) == pdTRUE) {
        distLeftFoot = lengthCalf*sin(radians(data.kneeAngleL))
                        + lengthThigh*sin(radians(data.hipAngleL))
                        - lengthTorso/2.0*sin(radians(data.backAngle));
        distRightFoot = lengthCalf*sin(radians(data.kneeAngleR))
                        + lengthThigh*sin(radians(data.hipAngleR))
                        - lengthTorso/2.0*sin(radians(data.backAngle));
        xSemaphoreGive(morphologyMutex);
    }               
}


void Logic::getNormalForces(RequiredData data, float& fnRight, float& fnLeft)
{
    float distLeftFoot;
    float distRightFoot;
    getDistanceFromCenterMass(data, distLeftFoot, distRightFoot);

    float totalDist = abs(distLeftFoot - distRightFoot);
    
    if (totalDist < abs(distLeftFoot) || totalDist < abs(distRightFoot))
    {
        //TODO crab mode
        totalDist = 0.0;
    }

    
/*     Serial.print("Dist left foot : ");
    Serial.println(distLeftFoot);
    Serial.print("Dist right foot : ");
    Serial.println(distRightFoot);
    Serial.print("Dist total : ");
    Serial.println(totalDist); 
    Serial.print("force t :");
    Serial.println(forceTorso);  */
                    
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


void Logic::valideTorque(RequiredData data, float (&torque)[NB_MOTOR])
{  
    if (limitAngleHip(data.backAngle, data.hipAngleL))
    {
        Serial.print("HIP LEFT not good : ");
        Serial.println(data.backAngle + data.hipAngleL);
        torque[HIP_LEFT] = 0.0;
    }
    if (limitAngleHip(data.backAngle, data.hipAngleR))
    {
        Serial.print("HIP RIGHT not good : ");
        Serial.println(data.backAngle + data.hipAngleR);
        torque[HIP_RIGHT] = 0.0;
    }
    if (limitAngleKnee(data.backAngle, data.hipAngleL, data.kneeAngleL))
    {
        Serial.print("KNEE LEFT not good : ");
        Serial.println(data.backAngle + data.hipAngleL + data.kneeAngleL);
        torque[KNEE_LEFT] = 0.0;
    }
    if (limitAngleKnee(data.backAngle, data.hipAngleR, data.kneeAngleR))
    {
        Serial.print("KNEE right not good : ");
        Serial.println(data.backAngle + data.hipAngleR + data.kneeAngleR);
        torque[KNEE_RIGHT] = 0.0;
    }
}


bool Logic::limitAngleHip(float angleBack, float angleHip)
{
    const uint8_t max_angle_positive = 80;
    const int8_t max_angle_negative = -70;
    const float max_angle_hip[2] = {angleBack + max_angle_positive, 
                                    angleBack + max_angle_negative};
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

    const float max_angle_hip[2] = {angleBack + angleHip + max_angle_positive, 
                                    angleBack + angleHip + max_angle_negative};

    if (angleKnee > max_angle_hip[0])
        return true;
    if (angleKnee < max_angle_hip[1])
        return true;
    else 
        return false;
}