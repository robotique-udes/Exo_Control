/**
 * @file IContext.hpp
 * @brief Declaration of the IContext interface class
 * 
 * @author Eloi Charbonneau
 * @date 2026-09-23
 */
#ifndef BIOGENIUS_CONTEXT_HPP
#define BIOGENIUS_CONTEXT_HPP

#include "Config.hpp"

class IContext
{
public:
	/**
	 * @brief Calculate the context validity
	 * 
	 * @param p_angles Joint angles
	 * @return value between 0-1 for context validity
	 */
	virtual float computeContextValidity(const float p_angles[exo_config::bnos::AMOUNT]) = 0;

    /**
     * @brief Calculate the torque values for all motors from the current joint angles.
     *
     * @param[in] p_angles Joint angles
     * @param[in] p_grounded Support state for each leg, where true indicates the foot is grounded.
     * @param[out] p_torque Output torque array filled with the computed motor commands.
     */
    virtual void calculateTorque(const float p_angles[exo_config::bnos::AMOUNT], 
                            const bool p_grounded[exo_config::bnos::NB_LEG],
                            float (&p_torque)[exo_config::motors::AMOUNT]) = 0;

};

#endif
