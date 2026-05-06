#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>


#define GRAVITY 9.81
#define EXO_MASS 10 //kg


struct RequiredData {
    float hipAngleL;
    float hipAngleR;
    float kneeAngleL;
    float kneeAngleR;
    float backAngle;
    bool groundedL;
    bool groundedR;
}; 


struct Position {
    float x;
    float y;
}; 

class Logic
{
private:

    float userHeight;
    float userMass;

    float lengthTorso;
    float lengthThigh;
    float lengthCalf;

    float forceTorso;
    float forceThigh;
    float forceCalf;

    void setMorphology(float height, float mass);
    
    void calculateTorqueAirborne(float angleHip, float angleKnee, bool grounded, float torque[2]);
    void calculateTorqueGrounded(float angleHip, float angleKnee, float fg, float torque[2]);
    void getDistanceFromCenterMass(RequiredData data, float& distLeftFoot, float& distRightFoot);
    void getNormalForces(RequiredData data, float& fnRight, float& fnLeft);

    // -------------------------- UTILITIES --------------------------
    /**
     * @brief Cap input value to max/min value
     * @param val Value to map (float or int)
     * @param cap max/min reachable value (float or int)
     */
    template <typename T>
    void limitMinMax(T &val, T cap);

public :
    void calculateTorque(RequiredData data, float (&torque)[4]);
    Logic();
};

#endif