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
    float exoForce = EXO_MASS*GRAVITY;

    lengthTorso = 0.47*userHeight;   
    lengthThigh = 0.245*userHeight;   
    lengthCalf = 0.285*userHeight;    

    forceTorso = 0.678*gravitationalForce + exoForce; 
    forceThigh = 0.1*gravitationalForce;   
    forceCalf = 0.061*gravitationalForce;  
}


void Logic::calculateTorque(RequiredData data)
{
    float torque[4] = {0,0,0,0};
   
   if (data.groundedL && data.groundedR)
    {
        float fnRight = 0.0;
        float fnLeft = 0.0;
        getNormalForces(data, fnRight, fnLeft);
        calculateTorqueGrounded(data.backAngle,data.hipAngleL, fnLeft, torque);
        calculateTorqueGrounded(data.backAngle,data.hipAngleR, fnRight, torque+2);
    }
    else if (data.groundedL || data.groundedR)
    { 
        calculateTorqueLeg(data.hipAngleL,data.kneeAngleL, data.groundedL, torque);
        calculateTorqueLeg(data.hipAngleR,data.kneeAngleR, data.groundedR, torque+2);
    }

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


void Logic::calculateTorqueGrounded(float angleTorso, float angleThigh, float forceOnLeg, float torque[2])
{
    float torqueHip = lengthTorso/2.0*sin(radians(angleTorso)) * forceOnLeg;
    float torqueKnee = -lengthThigh*sin(radians(angleThigh))*(0.5*forceThigh + forceOnLeg) + torqueHip;

    torque[0] = torqueKnee;
    torque[1] = torqueHip;
}


void Logic::getDistanceFromCenterMass(RequiredData data, float& distLeftFoot, float& distRightFoot)
{
    distLeftFoot = lengthCalf*sin(radians(data.kneeAngleL))
                    + lengthThigh*sin(radians(data.hipAngleL))
                    - lengthTorso/2.0*sin(radians(data.backAngle));
    distRightFoot = lengthCalf*sin(radians(data.kneeAngleR))
                    + lengthThigh*sin(radians(data.hipAngleR))
                    - lengthTorso/2.0*sin(radians(data.backAngle));
                    
}


void Logic::getNormalForces(RequiredData data, float& fnRight, float& fnLeft)
{
    float distLeftFoot;
    float distRightFoot;
    getDistanceFromCenterMass(data, distLeftFoot, distRightFoot);
    float totalDist = abs(distLeftFoot - distRightFoot);

/*     Serial.print("Dist left foot : ");
    Serial.println(distLeftFoot);
    Serial.print("Dist right foot : ");
    Serial.println(distRightFoot);
    Serial.print("Dist total : ");
    Serial.println(totalDist); */

    if (totalDist == 0)
    {
        fnRight = forceTorso/2.0;
        fnLeft = forceTorso/2.0;
        return;
    }

    fnRight = forceTorso*distLeftFoot/totalDist;
    fnLeft = forceTorso*distRightFoot/totalDist;
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

