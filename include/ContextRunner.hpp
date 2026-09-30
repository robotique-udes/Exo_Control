/**
 * @file ContextRunner.hpp
 * @brief Declaration of the ContextRunner class
 * 
 * @author Éloi Charbonneau
 * @date 2026-09-30
 */
#ifndef CONTEXT_RUNNER_HPP
#define CONTEXT_RUNNER_HPP

#include "IContext.hpp"

/**
 * @brief Selects the most valid registered context and computes the motor torque with it.
 */
class ContextRunner
{
public:
    /**
     * @brief Register a context that can be selected by update().
     *
     * @param[in] p_context Context to register. It is not owned and must outlive the runner.
     * @retval true The context was registered.
     * @retval false The context is null or exo_config::context::MAX_CONTEXTS is reached.
     */
    bool registerContext(IContext* p_context);

    /**
     * @brief Select the context with the highest validity and compute the torque with it.
     *
     * @details Every registered context computes its validity on each call, so contexts
     *          that track the movement over time keep receiving samples. On equal validity,
     *          the context registered first is selected. The torque is zero if no context is registered.
     *
     * @param[in] p_angles Joint angles measured by the BNO sensors in degrees.
     * @param[in] p_grounded Support state for each leg, where true indicates the foot is grounded.
     * @param[out] p_torque Output torque array filled with the computed motor commands.
     */
    void update(const float p_angles[exo_config::bnos::AMOUNT], 
                const bool p_grounded[exo_config::bnos::NB_LEG],
                float (&p_torque)[exo_config::motors::AMOUNT]);

private:
    IContext* m_contexts[exo_config::context::MAX_CONTEXTS] = {nullptr}; ///< Registered contexts, in registration order.
    uint8_t m_contextCount = 0; ///< Number of registered contexts.
};

#endif
