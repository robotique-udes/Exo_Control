/**
 * @file movingAverage.h
 * @brief Implementation of the MovingAverage class
 * 
 * @author Samuel Savaria
 * @date 2026-06-09
 */
#include "movingAverage.h"

MovingAverage::MovingAverage() {}

MovingAverage::MovingAverage(unsigned long new_period)
{
    setPeriod(new_period);
    reset();
}

void MovingAverage::reset()
{
    dataPoints = std::queue<DataPoint>(); // Destroy the old queue and start anew
    DataPoint head = {0.0f, 0};
    DataPoint tail = {0.0f, 0};

    currentAverage = 0.0f;
}

void MovingAverage::setPeriod(unsigned long new_period)
{
    period = new_period;
}

float MovingAverage::addValue(float value, unsigned long time)
{
    // Reusable variables and edge cases
    unsigned long timeSpan = 0;
    if(dataPoints.empty()) // Edge case for the first value
    {
        dataPoints.push({value, time});
        head = {value, time};

        return 0.0f;
    }

    // Add the new portion to the average
    timeSpan = time - head.time;
    currentAverage += head.value * ((float)timeSpan / period);

    // Removes the old portion to the average
    bool newTail = false;
    unsigned long previousTimeSpan = 0; // In case multiple value needs to be popped
    while(dataPoints.front().time <= time - period)
    {
        timeSpan = dataPoints.front().time - (head.time - period) - previousTimeSpan; // Subtract the previous timeSpan to avoid counting the same timeSpan twice
        previousTimeSpan += timeSpan;
        currentAverage -= tail.value * ((float)timeSpan / period);
        tail = dataPoints.front();
        dataPoints.pop();
        newTail = true;
    }

    if(newTail)
    {
        timeSpan = (time - period) - tail.time;
    }
    else
    {
        timeSpan = (time - period) - (head.time - period);
    }
    currentAverage -= tail.value * ((float)timeSpan / period);
        
    // Book keeping
    dataPoints.push({value, time});
    head = {value, time};

    return currentAverage;
}