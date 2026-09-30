/**
 * @file ContextRunner.cpp
 * @brief Implementation of the ContextRunner class
 * 
 * @author Éloi Charbonneau
 * @date 2026-09-30
 */

#include "ContextRunner.hpp"

bool ContextRunner::registerContext(IContext* p_context)
{
    if (p_context == nullptr || m_contextCount >= exo_config::context::MAX_CONTEXTS)
    {
        return false;
    }

    m_contexts[m_contextCount] = p_context;
    m_contextCount++;
    return true;
}

void ContextRunner::update(const float p_angles[exo_config::bnos::AMOUNT], 
                           const bool p_grounded[exo_config::bnos::NB_LEG],
                           float (&p_torque)[exo_config::motors::AMOUNT])
{
    IContext* bestContext = nullptr;
    float bestValidity = -1.0f;

    for (uint8_t i = 0; i < m_contextCount; ++i)
    {
        IContext* context = m_contexts[i];
        const float validity = context->computeContextValidity(p_angles);
        if (validity > bestValidity)
        {
            bestValidity = validity;
            bestContext = context;
        }
    }

    if (bestContext == nullptr)
    {
        for (uint8_t motor = 0; motor < exo_config::motors::AMOUNT; ++motor)
        {
            p_torque[motor] = 0.0f;
        }
        return;
    }

    bestContext->computeTorque(p_angles, p_grounded, p_torque);
}
