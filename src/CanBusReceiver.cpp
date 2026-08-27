/**
 * @file CanBusReceiver.cpp
 * @brief Implementation of the CanBusReceiver class
 * 
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */
#include "CanBusReceiver.hpp"

bool CanBusReceiver::begin(int p_txPin, int p_rxPin, int p_speedKbps, uint16_t p_rxQueueSize, uint16_t p_txQueueSize)
{
    return ESP32Can.begin(ESP32Can.convertSpeed(p_speedKbps), p_txPin, p_rxPin, p_rxQueueSize, p_txQueueSize);
}

void CanBusReceiver::subscribe(FrameCallback p_callback)
{
    m_callbacks.push_back(std::move(p_callback));
}

void CanBusReceiver::update()
{
    CanFrame frame;
    while(ESP32Can.readFrame(&frame, 0))
    {
        for(FrameCallback& callback : m_callbacks)
        {
            callback(frame);
        }
    }
}
