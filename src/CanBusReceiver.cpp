/**
 * @file CanBusReceiver.cpp
 * @brief Implementation of the CanBusReceiver class
 * 
 * @author Eloi Charbonneau
 * @date 2026-08-25
 */
#include "CanBusReceiver.hpp"
#include <assert.h>

bool CanBusReceiver::begin(int p_txPin, int p_rxPin, int p_speedKbps, uint16_t p_rxQueueSize, uint16_t p_txQueueSize)
{
    return ESP32Can.begin(ESP32Can.convertSpeed(p_speedKbps), p_txPin, p_rxPin, p_rxQueueSize, p_txQueueSize);
}

void CanBusReceiver::subscribe(FrameCallback p_callback, bool p_identifierFiltered, uint32_t p_id)
{
    assert(m_subscriberCount < MAX_SUBSCRIBERS);

    m_subscribers[m_subscriberCount++] = Subscriber{p_id, p_identifierFiltered, std::move(p_callback)};
}

void CanBusReceiver::update()
{
    CanFrame frame;
    while(ESP32Can.readFrame(&frame, 0))
    {
        for(size_t i = 0; i < m_subscriberCount; ++i)
        {
            Subscriber& subscriber = m_subscribers[i];
            if((subscriber.identifierFiltered && subscriber.id == frame.identifier)
                 || !subscriber.identifierFiltered)
            {
                subscriber.callback(frame);
            }
        }
    }
}
