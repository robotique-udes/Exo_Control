/**
 * @file ContextStatic.hpp
 * @brief Declaration of the ContextStatic class
 * 
 * @author Gabriel Desrochers, Éloi Charbonneau
 * @date 2026-09-26
 */
#ifndef CONTEXT_STATIC_HPP
#define CONTEXT_STATIC_HPP

#include "IContext.hpp"
#include "UserMorphology.hpp"
#include "utils/MovingAverage.hpp"

/**
 * @brief Static biomechanical model computing torque from posture and support-state data.
 *
 * @details Uses the shared UserMorphology and joint-angle measurements to derive
 *          torque values for the exoskeleton motors in both grounded and airborne phases.
 */
class ContextStatic : public IContext
{
public:
	/**
	 * @brief Construct a new ContextStatic object.
	 */
	ContextStatic();

	/**
	 * @brief Calculate the context validity
	 * 
	 * @details Computes, for each of the thigh and shin angles, the average angle change between
	 *          samples over exo_config::context::ANGLE_DELTA_WINDOW. The validity is the lowest value
	 *          of (1 - average change in degrees) among those joints, clamped between 0 and 1.
	 *          Must be called once per control cycle.
	 * 
	 * @param p_angles Joint angles in degrees
	 * @return value between 0-1 for context validity
	 */
	float computeContextValidity(const float p_angles[exo_config::bnos::AMOUNT]) override;

    /**
     * @brief Calculate the torque values for all motors from the current joint angles.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[in] p_grounded Support state for each leg, where true indicates the foot is grounded.
     * @param[out] p_torque Output torque array filled with the computed motor commands.
     */
    void computeTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                            const bool p_grounded[exo_config::bnos::NB_LEG],
                            float (&p_torque)[exo_config::motors::AMOUNT]) override;

private:
    /**
     * @brief Calculate torque commands for a leg in the airborne phase.
     *
     * @param[in] p_angleHip Hip angle in degrees.
     * @param[in] p_angleKnee Knee angle in degrees.
     * @param[out] p_torqueHip Computed hip torque in Nm.
     * @param[out] p_torqueKnee Computed knee torque in Nm.
     */
    void calculateTorqueAirborne(float p_angleHip, float p_angleKnee, float &p_torqueHip, float &p_torqueKnee);

    /**
     * @brief Calculate torque commands for a leg in the grounded phase.
     *
     * @param[in] p_angleTorso Torso angle in degrees.
     * @param[in] p_angleThigh Thigh angle in degrees.
     * @param[in] p_forceOnLeg Force applied on the leg in Newtons.
     * @param[out] p_torqueHip Computed hip torque in Nm.
     * @param[out] p_torqueKnee Computed knee torque in Nm.
     */
    void calculateTorqueGrounded(float p_angleTorso, float p_angleThigh, float p_forceOnLeg,
                                    float &p_torqueHip, float &p_torqueKnee);

    /**
     * @brief Calculate the horizontal distance between the center of mass and a foot.
     *
     * @param[in] p_angleTorso Torso angle in degrees.
     * @param[in] p_angleThigh Thigh angle in degrees.
     * @param[in] p_angleShin Shin angle in degrees.
     * @return Distance in meters.
     */
    float calculateDistanceFromCenterMass(float p_angleTorso, float p_angleThigh, float p_angleShin);

    /**
     * @brief Get the distance of each foot from the center of mass.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[out] p_distLeftFoot Distance to the left foot in meters.
     * @param[out] p_distRightFoot Distance to the right foot in meters.
     */
    void getDistanceFromCenterMass(const float p_angles[exo_config::bnos::AMOUNT], 
                                    float& p_distLeftFoot, float& p_distRightFoot);

    /**
     * @brief Get the normal force applied on each foot.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[out] p_fnRight Normal force on the right foot in Newtons.
     * @param[out] p_fnLeft Normal force on the left foot in Newtons.
     */
    void getNormalForces(const float p_angles[exo_config::bnos::AMOUNT], 
                            float& p_fnRight, float& p_fnLeft);

    /**
     * @brief Zero the torque of every joint whose angle is out of its allowed range.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[out] p_torque Torque values to validate and adjust in Nm.
     */
    void validateTorque(const float p_angles[exo_config::bnos::AMOUNT], float (&p_torque)[exo_config::motors::AMOUNT]);

    /**
     * @brief Check whether the hip angle is out of the allowed range.
     *
     * @param[in] p_angleBack Back angle in degrees.
     * @param[in] p_angleHip Hip angle in degrees.
     * @retval true The hip angle is out of the allowed range.
     * @retval false The hip angle is within the allowed range.
     */
    bool isHipAngleOutOfLimit(float p_angleBack, float p_angleHip);

    /**
     * @brief Check whether the knee angle is out of the allowed range.
     *
     * @param[in] p_angleBack Back angle in degrees.
     * @param[in] p_angleHip Hip angle in degrees.
     * @param[in] p_angleKnee Knee angle in degrees.
     * @retval true The knee angle is out of the allowed range.
     * @retval false The knee angle is within the allowed range.
     */
    bool isKneeAngleOutOfLimit(float p_angleBack, float p_angleHip, float p_angleKnee);

    UserMorphology& m_morphology = UserMorphology::getInstance(); ///< Shared user morphology.
    MovingAverage m_deltaAverages[exo_config::bnos::AMOUNT]; ///< Average angle change per BNO [deg].
    float m_previousAngles[exo_config::bnos::AMOUNT] = {0}; ///< Angles of the previous call per BNO [deg].
    bool m_hasPreviousAngles = false; ///< True once m_previousAngles holds real measurements.
};

#endif
