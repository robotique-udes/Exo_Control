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

    /**
     * @brief Asks the observer whether it is interested in a frame with the given CAN identifier
     *
     * @details Lets the dispatcher filter frames before calling notify(), so each observer only
     *          ever parses frames meant for it
     *
     * @param[in] id The CAN identifier of the frame
     * 
     * @return True if the observer wants to be notified of frames with this identifier
     */
    virtual bool wantsFrame(uint32_t id) const = 0;
};

#endif
