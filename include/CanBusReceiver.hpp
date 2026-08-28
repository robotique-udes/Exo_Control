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
#include <functional>

/**
 * @brief Reads incoming CAN frames and dispatches them to registered observers
 *
 * @details Any component interested in CAN frames (motors, sensors, etc.) registers a
 *          callback via subscribe(), optionally restricting it to a single CAN ID.
 *          Every frame read from the bus during update() is passed to every subscriber
 *          whose filter matches (or to every subscriber that isn't filtered). This
 *          decouples reading the bus from the components that consume the data, so new
 *          CAN devices can be added without changing how the bus itself is read.
 *
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */
class CanBusReceiver
{
public:
    using FrameCallback = std::function<void(const CanFrame&)>;

    /// Maximum number of callbacks that can be registered via subscribe()
    static constexpr size_t MAX_SUBSCRIBERS = 8;

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
     * @brief Registers a callback invoked with frames read from the bus
     *
     * @param[in] p_callback           The function to call with each matching CAN frame
     * @param[in] p_identifierFiltered If true, the callback only receives frames whose identifier matches p_id
     * @param[in] p_id                 The CAN identifier to filter on when p_identifierFiltered is true
     */
    void subscribe(FrameCallback p_callback, bool p_identifierFiltered, uint32_t p_id);

    /**
     * @brief Reads all pending CAN frames and dispatches each one to every matching registered callback
     */
    void update();

private:
    /**
     * @brief A registered observer, optionally restricted to a single CAN identifier
     */
    struct Subscriber
    {
        uint32_t id;
        bool identifierFiltered;
        FrameCallback callback;
    };

    std::array<Subscriber, MAX_SUBSCRIBERS> m_subscribers;
    size_t m_subscriberCount = 0;
};

#endif
