#include "logic.h"


Logic::Logic()
{
    setMorphology(1.70, 70);
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
   
    calculateTorqueLeg(data.hipAngleL,data.kneeAngleL, data.groundedL, torque);
    calculateTorqueLeg(data.hipAngleR,data.kneeAngleR, data.groundedR, torque+2);
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

void Logic::calculateTorqueLeg(float angleHip, float angleKnee, bool grounded, float torque[2])
{
    float torqueKnee;
    float torqueHips;
    if (grounded)
    {
        torqueKnee = -1*((forceTorso/2.0 + forceThigh)*lengthCalf*sin(radians(angleKnee))
                        + forceCalf*lengthCalf/2.0*sin(radians(angleKnee)));

        torqueHips = torqueKnee + forceTorso/2.0*lengthThigh*sin(radians(angleHip))
                        + forceThigh*lengthThigh/2.0*sin(radians(angleHip));
    }
    else 
    {
        torqueKnee = forceCalf*lengthCalf/2.0*sin(radians(angleKnee));
        torqueHips = torqueKnee + forceThigh*lengthThigh/2.0*sin(radians(angleHip))
                        + forceCalf*(lengthThigh*sin(radians(angleHip)) + lengthCalf/2.0*sin(radians(angleKnee)));
    }
    
    torque[0] = torqueKnee;
    torque[1] = torqueHips;
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

