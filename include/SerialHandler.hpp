/**
 * @file SerialHandler.hpp
 * @brief Declaration of the SerialHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-07-18
 */
#ifndef BIOGENIUS_SERIAL_HANDLER_HPP
#define BIOGENIUS_SERIAL_HANDLER_HPP

#include <Arduino.h>
#include <array>

/**
 * @brief Manually controls the torque using the serial port
 *
 * @author Samuel Savaria
 * @date 2026-07-18
 */
class SerialHandler
{
public:
    /**
     * @brief Reads commands from the serial port 
     * 
     * @details The commands are single ASCII characters
     *          w: Increase the selected motor's torque by 1
     *          s: Decrease the selected motor's torque by 1
     *          d: Select the next motor ID
     *          a: Select the previous motor ID
     *          r: Restart the motherboard
     *
     * @return Array of torques. The index of the array corresponds to the CAN ID of the motor
     */
    std::array<float, 4> update();

private:
    /**
     * @brief Sets the specified motor's torque
     *
     * @param[in] torque The motor's new torque
     */
    void setTorque(float torque);

    /**
     * @brief Sets the selected motor
     *
     * @param[in] motorID The ID of the selected motor
     */
    void setMotorID(uint8_t motorID);

    /**
     * @brief Restarts the motherboard
     */
    void restart();

    std::array<float, 4> m_torques = {0, 0, 0, 0};
    uint8_t m_selectedMotorID = 0;
};

#endif