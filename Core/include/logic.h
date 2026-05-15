#ifndef LOGIC_H
#define LOGIC_H

#include <Arduino.h>
#include "config.h"

namespace  physics = app::config::physics;
namespace anatomy = app::config::anatomy;
namespace debug = app::config::debug;
namespace motor_config = app::config::motors;
namespace bno_config = app::config::bnos;


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
    
    void calculateTorqueAirborne(float angleHip, float angleKnee, bool grounded,
                                    float &torqueHip, float &torqueKnee);
    
    void calculateTorqueGrounded(float angleHip, float angleKnee, float fg,
                                    float &torqueHip, float &torqueKnee);

    void getDistanceFromCenterMass(const float angles[bno_config::amount], 
                                    float& distLeftFoot, float& distRightFoot);
    void getNormalForces(const float angles[bno_config::amount], 
                            float& fnRight, float& fnLeft);

    void valideTorque(const float angles[bno_config::amount], float (&torque)[motor_config::amount]);

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
    void calculateTorque(const float angles[bno_config::amount], 
                            const bool grounded[bno_config::nb_leg],
                            float (&torque)[motor_config::amount]);
};


template <typename T, typename U>
inline void Logic::limitMinMax(T &val, U cap) {
    if (val > (T)cap) val = (T)cap;
    if (val < -(T)cap) val = -(T)cap;
}
#endif