/**
 * @file MovingAverage.cpp
 * @brief Implementation of the MovingAverage class
 * 
 * @author Samuel Savaria
 * @date 2026-06-09
 */
#include "MovingAverage.h"

MovingAverage::MovingAverage() {}

MovingAverage::MovingAverage(unsigned long p_period)
{
    setPeriod(p_period);
    reset();
}

void MovingAverage::reset()
{
    m_dataPoints = std::queue<DataPoint>(); // Destroy the old queue and start anew
    m_head = {0.0f, 0};
    m_tail = {0.0f, 0};

    m_currentAverage = 0.0f;
}

void MovingAverage::setPeriod(unsigned long p_period)
{
    m_period = p_period;
}

float MovingAverage::addValue(float p_value, unsigned long p_time)
{
    // Reusable variables and edge cases
    unsigned long timeSpan = 0;
    if(m_dataPoints.empty()) // Edge case for the first value
    {
        m_dataPoints.push({p_value, p_time});
        m_head = {p_value, p_time};

        return 0.0f;
    }

    // Add the new portion to the average
    timeSpan = p_time - m_head.time;
    m_currentAverage += m_head.value * ((float)timeSpan / m_period);

    // Removes the old portion to the average
    bool newTail = false;
    unsigned long previousTimeSpan = 0; // In case multiple value needs to be popped
    while(m_dataPoints.front().time <= p_time - m_period)
    {
        timeSpan = m_dataPoints.front().time - (m_head.time - m_period) - previousTimeSpan; // Subtract the previous timeSpan to avoid counting the same timeSpan twice
        previousTimeSpan += timeSpan;
        m_currentAverage -= m_tail.value * ((float)timeSpan / m_period);
        m_tail = m_dataPoints.front();
        m_dataPoints.pop();
        newTail = true;
    }

    if(newTail)
    {
        timeSpan = (p_time - m_period) - m_tail.time;
    }
    else
    {
        timeSpan = (p_time - m_period) - (m_head.time - m_period);
    }
    m_currentAverage -= m_tail.value * ((float)timeSpan / m_period);
        
    // Book keeping
    m_head = {p_value, p_time};
    m_dataPoints.push(m_head);

    return m_currentAverage;
}