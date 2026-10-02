/**
 * @file CanBusReceiver.hpp
 * @brief Declaration of the CanBusReceiver class
 * 
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */

#ifndef BIOGENIUS_CAN_BUS_HANDLER_HPP
#define BIOGENIUS_CAN_BUS_HANDLER_HPP

#include <ESP32-TWAI-CAN.hpp>
#include <array>

#include "ICanObserver.hpp"

/**
 * @brief Reads incoming CAN frames and dispatches them to registered observers
 *
 * @details Any component interested in CAN frames (motors, sensors, etc.) registers itself
 *          via addObserver(). Every frame read from the bus during update() is passed to
 *          every registered observer's notify(). This decouples reading the bus from the
 *          components that consume the data, so new CAN devices can be added without
 *          changing how the bus itself is read.
 *
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */
class CanBusReceiver
{
public:
    /// Maximum number of observers that can be registered via addObserver()
    static constexpr size_t MAX_SUBSCRIBERS = 4;

    /**
     * @brief Starts the CAN bus
     *
     * @param[in] p_txPin        The CAN TX pin
     * @param[in] p_rxPin        The CAN RX pin
     * @param[in] p_speedKbps    The bus speed in kbit/s
     * @param[in] p_rxQueueSize  The size of the receive queue
     * @param[in] p_txQueueSize  The size of the transmit queue
     * 
     * @return True if the bus started successfully
     */
    bool begin(int p_txPin, int p_rxPin, int p_speedKbps, uint16_t p_rxQueueSize = 5, uint16_t p_txQueueSize = 5);

    /**
     * @brief Registers an observer notified with every frame read from the bus
     *
     * @param[in] p_observer The observer to notify with each CAN frame
     */
    void addObserver(ICanObserver* p_observer);

    /**
     * @brief Reads all pending CAN frames and dispatches each one to every registered observer
     */
    void update();

private:
    std::array<ICanObserver*, MAX_SUBSCRIBERS> m_observers;
    size_t m_observerCount = 0;
};

#endif
