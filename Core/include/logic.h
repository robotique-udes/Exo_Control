#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>


#define GRAVITY 9.81
#define EXO_MASS 10 //kg

#define NB_MOTOR 4
#define KNEE_LEFT 0
#define HIP_LEFT 1
#define KNEE_RIGHT 2
#define HIP_RIGHT 3


#define MAX_TORQUE 1.0f //N*m
#define TORQUE_MULTIPLIER 0.1




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

    SemaphoreHandle_t morphologyMutex;

    float userHeight;
    float userMass;

    float lengthTorso;
    float lengthThigh;
    float lengthCalf;

    float forceTorso;
    float forceThigh;
    float forceCalf;
    
    void calculateTorqueAirborne(float angleHip, float angleKnee, bool grounded, float torque[2]);
    void calculateTorqueGrounded(float angleHip, float angleKnee, float fg, float torque[2]);
    void getDistanceFromCenterMass(RequiredData data, float& distLeftFoot, float& distRightFoot);
    void getNormalForces(RequiredData data, float& fnRight, float& fnLeft);
    void valideTorque(RequiredData data, float (&torque)[NB_MOTOR]);
    bool limitAngleHip(float angleBack, float angleHip);
    bool limitAngleKnee(float angleBack, float angleHip, float angleKnee);

    // -------------------------- UTILITIES --------------------------
    /**
     * @brief Cap input value to max/min value
     * @param val Value to map (float or int)
     * @param cap max/min reachable value (float or int)
     */
    template <typename T, typename U>
    static void limitMinMax(T &val, U cap);

public :
    Logic();
    void setMorphology(int height, int mass);
    void calculateTorque(RequiredData data, float (&torque)[NB_MOTOR]);
};


template <typename T, typename U>
inline void Logic::limitMinMax(T &val, U cap) {
    if (val > (T)cap) val = (T)cap;
    if (val < -(T)cap) val = -(T)cap;
}
#endif