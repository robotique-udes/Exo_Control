#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <Arduino.h>
#include "Config.hpp"

/**
 * @brief Computes motor torque commands from posture and support-state data.
 *
 * @details The Logic class uses biomechanical parameters and joint-angle measurements to derive
 *          torque values for the exoskeleton motors in both grounded and airborne phases.
 */
class Logic
{
public :
    /**
     * @brief Construct a new Logic object.
     */
    Logic();

    /**
     * @brief Set the user morphology used by the torque calculation model.
     *
     * @param[in] p_height User height expressed in centimeters.
     * @param[in] p_mass User mass expressed in kilograms.
     */
    void setMorphology(int p_height, int p_mass);

    /**
     * @brief Calculate the torque values for all motors from the current joint angles.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors for the available joints.
     * @param[in] p_grounded Support state for each leg, where true indicates the foot is grounded.
     * @param[out] p_torque Output torque array filled with the computed motor commands.
     */
    void calculateTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                            const bool p_grounded[exo_config::bnos::NB_LEG],
                            float (&p_torque)[exo_config::motors::AMOUNT]);

private:

    /**
     * @brief Calculate torque commands for the airborne phase.
     *
     * @param[in] p_angleHip Hip angle used for the current leg state.
     * @param[in] p_angleKnee Knee angle used for the current leg state.
     * @param[out] p_torqueHip Computed hip torque in Nm.
     * @param[out] p_torqueKnee Computed knee torque in Nm.
     */
    void calculateTorqueAirborne(float p_angleHip, float p_angleKnee, float &p_torqueHip, float &p_torqueKnee);
    
    /**
     * @brief Calculate torque commands for the grounded phase.
     *
     * @param[in] p_angleHip Hip angle used for the current leg state.
     * @param[in] p_angleKnee Knee angle used for the current leg state.
     * @param[in] p_forceGround Force applied at the ground contact point.
     * @param[out] p_torqueHip Computed hip torque in Nm.
     * @param[out] p_torqueKnee Computed knee torque in Nm.
     */
    void calculateTorqueGrounded(float p_angleHip, float p_angleKnee, float p_forceGround,
                                    float &p_torqueHip, float &p_torqueKnee);

    /**
     * @brief Estimate the distance of each foot from the center of mass.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors.
     * @param[out] p_distLeftFoot Estimated distance from the center of mass to the left foot in meters.
     * @param[out] p_distRightFoot Estimated distance from the center of mass to the right foot in meters.
     */
    void getDistanceFromCenterMass(const float p_angles[exo_config::bnos::AMOUNT], 
                                    float& p_distLeftFoot, float& p_distRightFoot);

    /**
     * @brief Compute the normal forces applied to each foot in.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[out] p_fnRight Normal force on the right foot in Newtons.
     * @param[out] p_fnLeft Normal force on the left foot in Newtons.
     */
    void getNormalForces(const float p_angles[exo_config::bnos::AMOUNT], 
                            float& p_fnRight, float& p_fnLeft);

    /**
     * @brief Validate and adjust the computed torque values before they are sent to the motors.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[out] p_torque Torque values to validate and adjust in Nm.
     */
    void valideTorque(const float p_angles[exo_config::bnos::AMOUNT], float (&p_torque)[exo_config::motors::AMOUNT]);

    /**
     * @brief Check whether the hip angle remains within the allowed range.
     *
     * @param[in] p_angleBack Back angle used for the limit check in degrees.
     * @param[in] p_angleHip Hip angle to validate in degrees.
     * @return true if the angle stays within the allowed range, false otherwise.
     */
    bool limitAngleHip(float p_angleBack, float p_angleHip);

    /**
     * @brief Check whether the knee angle remains within the allowed range.
     *
     * @param[in] p_angleBack Back angle used for the limit check in degrees.
     * @param[in] p_angleHip Hip angle used for the limit check in degrees.
     * @param[in] p_angleKnee Knee angle to validate in degrees.
     * @return true if the angle stays within the allowed range, false otherwise.
     */
    bool limitAngleKnee(float p_angleBack, float p_angleHip, float p_angleKnee);

    SemaphoreHandle_t m_morphologyMutex; ///< Mutex used to protect morphology values from concurrent HMI access.
    float m_userHeight;   ///<  User height used to compute the torque model [cm].
    float m_userMass;     ///<  User mass used to compute the torque model [kg].
    float m_lengthTorso;  ///<  Length of the torso segment used in the biomechanical model [m].
    float m_lengthThigh;  ///<  Length of the thigh segment used in the biomechanical model [m].
    float m_lengthCalf;   ///<  Length of the calf segment used in the biomechanical model [m].
    float m_forceTorso;   ///<  Estimated force applied on the torso segment [N].
    float m_forceThigh;   ///<  Estimated force applied on the thigh segment [N].
    float m_forceCalf;    ///<  Estimated force applied on the calf segment [N].
};

#endif