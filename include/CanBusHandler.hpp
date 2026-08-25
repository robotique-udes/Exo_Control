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
#include <functional>
#include <vector>

/**
 * @brief Reads incoming CAN frames and dispatches them to registered observers
 *
 * @details Any component interested in CAN frames (motors, sensors, etc.) registers a
 *          callback via subscribe(). Every frame read from the bus during update() is
 *          passed to all registered callbacks; each callback is responsible for
 *          filtering the frames it cares about (e.g. by CAN ID). This decouples reading
 *          the bus from the components that consume the data, so new CAN devices can be
 *          added without changing how the bus itself is read.
 *
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */
class CanBusReceiver
{
public:
    using FrameCallback = std::function<void(const CanFrame&)>;

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
     * @brief Registers a callback invoked with every frame read from the bus
     *
     * @param[in] p_callback The function to call with each received CAN frame
     */
    void subscribe(FrameCallback p_callback);

    /**
     * @brief Reads all pending CAN frames and dispatches each one to every registered callback
     */
    void update();

private:
    std::vector<FrameCallback> m_callbacks;
};

#endif
