/**
 * @file ICanObserver.hpp
 * @brief Declaration of the ICanObserver interface class
 * 
 * @author Eloi Charbonneau
 * @date 2026-09-03
 */
#ifndef BIOGENIUS_CAN_OBSERVER_HPP
#define BIOGENIUS_CAN_OBSERVER_HPP

#include <ESP32-TWAI-CAN.hpp>

/**
 * @brief Interface for an object that reacts to notifications of incoming CAN frames
 *
 */
class ICanObserver
{
public:
    /**
     * @brief Called by the subject to notify this observer of an update
     *
     * @param[in] message The CAN message the observer is notified with
     */
    virtual void notify(const CanFrame& message) = 0;
};

#endif
