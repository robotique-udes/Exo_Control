#include "logic.h"


Logic::Logic()
{
    setMorphology(170, 70);
}



void Logic::setMorphology(float height, float mass)
{
    userHeight = height;
    userMass = mass;
    
    float gravitationalForce = userMass*GRAVITY;

    lengthTorso = 0.47*userHeight;   
    lengthThigh = 0.245*userHeight;   
    lengthCalf = 0.285*userHeight;    

    forceTorso = 0.678*gravitationalForce; 
    forceThigh = 0.1*gravitationalForce;   
    forceCalf = 0.061*gravitationalForce;  
}


void Logic::calculateTorque(RequiredData data)
{
    float torque[4] = {0,0,0,0};
    if (data.groundedL && data.groundedR)
        calculateTorqueGrounded(data, torque);
}

void Logic::calculateTorqueGrounded(RequiredData data, float torque[4])
{
    float sinBackAngle = sin(data.backAngle);
    float cosBackAngle = cos(data.backAngle);
    float sinHipAngleR = sin(data.hipAngleR);   
    float cosHipAngleR = cos(data.hipAngleR);   
    float sinHipAngleL = sin(data.hipAngleL);   
    float cosHipAngleL = cos(data.hipAngleL);   
    float sinKneeAngleR = sin(data.kneeAngleR);    
    float cosKneeAngleR = cos(data.kneeAngleR);    
    float sinKneeAngleL = sin(data.kneeAngleL);    
    float cosKneeAngleL = cos(data.kneeAngleL);    

    float gravitationalForce = userMass*GRAVITY;

    Position centerMass;
    centerMass.x = (0.5*forceTorso*lengthTorso*cosBackAngle 
                    + 0.5*forceThigh*lengthThigh*(cosHipAngleR + cosHipAngleL) 
                    + forceCalf*(lengthThigh*(cosHipAngleR + cosHipAngleL) 
                    + 0.5*lengthCalf*(cosKneeAngleR + cosKneeAngleL)))/gravitationalForce;
    centerMass.y = (0.5*forceTorso*lengthTorso*sinBackAngle 
                    + 0.5*forceThigh*lengthThigh*(sinHipAngleR + sinHipAngleL) 
                    + forceCalf*(lengthThigh*(sinHipAngleR + sinHipAngleL) 
                    + 0.5*lengthCalf*(sinKneeAngleR + sinKneeAngleL)))/gravitationalForce;

    Position positionFeetR;
    Position positionFeetL;
    positionFeetR.x = lengthThigh*cosHipAngleR + lengthCalf*cosKneeAngleR; 
    positionFeetR.y = lengthThigh*sinHipAngleR + lengthCalf*sinKneeAngleR;
    positionFeetL.x = lengthThigh*cosHipAngleL + lengthCalf*cosKneeAngleL; 
    positionFeetL.y = lengthThigh*sinHipAngleL + lengthCalf*sinKneeAngleL; 

    Position distFromCenterMassR;
    Position distFromCenterMassL;
    distFromCenterMassR.x = centerMass.x - positionFeetR.x; 
    distFromCenterMassR.y = centerMass.y - positionFeetR.y; 
    distFromCenterMassL.x = centerMass.x - positionFeetL.x; 
    distFromCenterMassL.y = centerMass.y - positionFeetL.y; 

    Position deltaPosFromCM;
    deltaPosFromCM.x = distFromCenterMassR.x - distFromCenterMassL.x;          
    deltaPosFromCM.y = distFromCenterMassR.y - distFromCenterMassL.y;   


    float calfDenom = lengthCalf*(sinKneeAngleL*(-1+gravitationalForce+0.5*forceCalf) 
                        + sinKneeAngleR*(1+0.5*forceCalf) 
                        + (cosKneeAngleL-cosKneeAngleR)
                        *(deltaPosFromCM.y+gravitationalForce*distFromCenterMassL.y)/deltaPosFromCM.x);
    float thighDenom = lengthThigh*(sinHipAngleL*(-1+gravitationalForce+forceCalf+0.5*forceThigh) 
                        + sinHipAngleR*(1+forceCalf+0.5*forceThigh) 
                        + (deltaPosFromCM.y+gravitationalForce*distFromCenterMassL.y)
                        *(cosHipAngleL-cosHipAngleR)/deltaPosFromCM.x);


    float normalForceR = 0.5*forceTorso*lengthTorso*sinBackAngle / (calfDenom + thighDenom);         
    float normalForceL = gravitationalForce - normalForceR;                                              
    float frictionForceR = -(normalForceR*deltaPosFromCM.y 
                            + gravitationalForce*distFromCenterMassL.y)/deltaPosFromCM.x;  
    float frictionForceL = -frictionForceR;                                                     


    float T_kr = lengthCalf*((normalForceR+0.5*forceCalf)*sinKneeAngleR 
                                + frictionForceR*cosKneeAngleR); // Torque right knee
    float T_kl = lengthCalf*((normalForceL+0.5*forceCalf)*sinKneeAngleL 
                                + frictionForceL*cosKneeAngleL); // Torque left knee

    float T_hr = T_kr + lengthThigh*((normalForceR+forceCalf+0.5*forceThigh)*sinHipAngleR 
                    + frictionForceR*cosHipAngleR); // Torque right hips
    float T_hl = T_kl + lengthThigh*((normalForceL+forceCalf+0.5*forceThigh)*sinHipAngleL 
                    + frictionForceL*cosHipAngleL); // Torque left hips

    torque[0] = T_kr;
    torque[1] = T_kl;
    torque[2] = T_hr;
    torque[3] = T_hl;
}

template <typename T>
void Logic::limitMinMax(T &val, T cap)
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