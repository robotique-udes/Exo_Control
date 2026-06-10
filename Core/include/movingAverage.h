/**
 * @file movingAverage.h
 * @brief Declaration of the MovingAverage class
 * 
 * @author Samuel Savaria
 * @date 2026-06-09
 */
#ifndef BIOGENIUS_MOVING_AVERAGE_H
#define BIOGENIUS_MOVING_AVERAGE_H

#include <Arduino.h>
#include <queue>

/**
 * @brief Provides a moving average whose window is a time period rather than a number of samples
 *
 * @author Samuel Savaria
 * @date 2026-06-09
 */
class MovingAverage
{
private:
    struct DataPoint
    {
        float value;
        unsigned long time;
    };

    std::queue<DataPoint> dataPoints;
    DataPoint head = {0.0f, 0}; // The last inserted value
    DataPoint tail = {0.0f, 0}; // The value who is partially exiting the window
    float currentAverage = 0.0f;
    unsigned long period = 1000; // In milliseconds. Must be the same data type as millis() https://docs.arduino.cc/language-reference/en/functions/time/millis/

public:
    /**
     * @brief Constructor
     */
    MovingAverage();

    /**
     * @brief Constructor
     *
     * @param period The length of the time window in milliseconds
     */
    MovingAverage(unsigned long period);

    /**
     * @brief Adds a value to the window and returns the updated average
     *
     * @param value The value to be added to the window
     * @param time  [optional] The time in milliseconds
     * 
     * @return The updated average
     */
    float addValue(float value, unsigned long time = millis());

    /**
     * @brief Resets the window. Discards existing values and resets the average to 0.
     */
    void reset();

    /**
     * @brief Set the length of the time window. It is recommended to reset the average after changing the period.
     *
     * @param period The length of the time window in milliseconds
     */
    void setPeriod(unsigned long period);
};

#endif