/**
 * @file BNOHandler.cpp
 * @brief Implementation of the BNOHandler class
 * 
 * @author Eloi Charbonneau
 * @date 2026-07-18
 */

#include "BNOHandler.hpp"
#include "Arduino.h"
#include <Wire.h>
#include <cmath>
#include <cstring>

BnoHandler::BnoHandler() 
{
    m_mux = Multiplex();

    // Keep this order, position in arrays is same as EnumBnoPosition value
    m_muxChannels[exo_config::bnos::LEFT_THIGH] = exo_config::path::LEFT_MOUSTACHE_CHANNEL;
    m_muxChannels[exo_config::bnos::RIGHT_THIGH] = exo_config::path::RIGHT_MOUSTACHE_CHANNEL;
    m_muxChannels[exo_config::bnos::LEFT_SHIN] = exo_config::path::LEFT_MOUSTACHE_CHANNEL;
    m_muxChannels[exo_config::bnos::RIGHT_SHIN] = exo_config::path::RIGHT_MOUSTACHE_CHANNEL;
    m_muxChannels[exo_config::bnos::EXO_BACK] = exo_config::path::BACK_CHANNEL;
    m_muxChannels[exo_config::bnos::MOBO] = exo_config::path::MOBO_CHANNEL;

    m_i2cAddresses[exo_config::bnos::LEFT_THIGH] = exo_config::path::ADDRESS_1;
    m_i2cAddresses[exo_config::bnos::RIGHT_THIGH] = exo_config::path::ADDRESS_1;
    m_i2cAddresses[exo_config::bnos::LEFT_SHIN] = exo_config::path::ADDRESS_2;
    m_i2cAddresses[exo_config::bnos::RIGHT_SHIN] = exo_config::path::ADDRESS_2;
    m_i2cAddresses[exo_config::bnos::EXO_BACK] = exo_config::path::ADDRESS_1;
    m_i2cAddresses[exo_config::bnos::MOBO] = exo_config::path::ADDRESS_2;

    m_bufferIndexLeft = 0;
    m_bufferIndexRight = 0;

    for(int i = 0; i < exo_config::bnos::BUFFER_SIZE; i++)
    {
        m_linAccelBufferLeft[i]=0;
        m_linAccelBufferRight[i]=0;
    }

    for (int i = 0; i < m_bnoDevices.size(); i++) 
    {
        m_bnoConnected[i] = false;
        m_bnoAngles[i] = 0;
    }
}

bool BnoHandler::begin() 
{
    int8_t connected = 0;

    if (exo_config::debug::BNO) 
    {
        Serial.print("===== BnoHandler STARTING =====\n");
    }

    for (size_t i = 0; i < m_bnoDevices.size(); ++i) 
    {
        m_mux.selectChannel(m_muxChannels[i]);

        const bool isConnected = m_bnoDevices[i].begin(m_i2cAddresses[i], Wire);
        setupReports(static_cast<uint8_t>(i));
        m_bnoConnected[i] = isConnected;

        if (isConnected)
        {
            connected++;
        }

        if (exo_config::debug::BNO) 
        {
            Serial.print("BNO ");
            Serial.print(i);
            Serial.print("\t");
            Serial.print("Connected: ");
            Serial.println(isConnected);
            delay(500);
        }
    }

    return connected > 0;
}

void BnoHandler::setupReports(uint8_t p_position) 
{
    const size_t index = bnoIndex(p_position);
    m_bnoDevices[index].enableGravity(10);
    m_bnoDevices[index].enableLinearAccelerometer(10);
}

bool BnoHandler::checkIfConnected(uint8_t p_position) 
{
    const size_t index = bnoIndex(p_position);
    m_mux.selectChannel(m_muxChannels[index]);
    Wire.beginTransmission(m_i2cAddresses[index]);
    const bool connected = (Wire.endTransmission() == 0);
    m_bnoConnected[index] = connected;
    return connected;
}

void BnoHandler::requestData() {
    for (size_t i = 0; i < exo_config::bnos::AMOUNT; ++i) 
    {
        if (!m_bnoConnected[i]) {
            continue;
        }

        m_mux.selectChannel(m_muxChannels[i]);

        if (m_bnoDevices[i].hasReset()) 
        {
            setupReports(static_cast<uint8_t>(i));
        }

        while (m_bnoDevices[i].dataAvailable()) 
        {
            float gX = m_bnoDevices[i].getGravityX();
            float gY = m_bnoDevices[i].getGravityY();
            float gZ = m_bnoDevices[i].getGravityZ();

            if (exo_config::bnos::MOBO == i) 
            {
                m_bnoAngles[i] = -1.0f * (degrees(atan2(gY, gX)) + 180.0f);
            } 
            else 
            {
                m_bnoAngles[i] = degrees(atan2(gX, gY)) + 180.0f;
            }

            if (m_bnoAngles[i] > 180.0f) 
            {
                m_bnoAngles[i] -= 360.0f;
            } 
            else if (m_bnoAngles[i] <= -180.0f) 
            {
                m_bnoAngles[i] = -1*m_bnoAngles[i];
            }

            if (exo_config::bnos::LEFT_THIGH == i || exo_config::bnos::LEFT_SHIN == i) 
            {
                m_bnoAngles[i] = -1.0f * m_bnoAngles[i];
            }

            float lax = 0.0f;
            float lay = 0.0f;
            float laz = 0.0f;
            uint8_t lac = 0;
            m_bnoDevices[i].getLinAccel(lax, lay, laz, lac);
            m_linearAccelerations[i].x = lax;
            m_linearAccelerations[i].y = lay;
            m_linearAccelerations[i].z = laz;
        }
    }

    m_lastUpdate = millis();
}

void BnoHandler::getAngle(float angles[exo_config::bnos::AMOUNT])
{
    memcpy(angles, &m_bnoAngles, sizeof(m_bnoAngles));
}

void BnoHandler::updateBuffer(uint8_t p_position) 
{
    const int16_t yAccel = getLinAccelYScaled(p_position);

    if (p_position == exo_config::bnos::LEFT_SHIN) 
    {
        m_linAccelBufferLeft[m_bufferIndexLeft] = abs(yAccel) < (exo_config::bnos::ACCEL_BUFFER_THRESHOLD + m_offset);
        if (m_bufferIndexLeft < (exo_config::bnos::BUFFER_SIZE - 1)) 
        {
            ++m_bufferIndexLeft;
        } 
        else 
        {
            m_bufferIndexLeft = 0;
        }
    } 
    else if (p_position == exo_config::bnos::RIGHT_SHIN) 
    {
        m_linAccelBufferRight[m_bufferIndexRight] = abs(yAccel) < (exo_config::bnos::ACCEL_BUFFER_THRESHOLD + m_offset);
        if (m_bufferIndexRight < (exo_config::bnos::BUFFER_SIZE - 1)) 
        {
            ++m_bufferIndexRight;
        } 
        else
        {
            m_bufferIndexRight = 0;
        }
    }
}

void BnoHandler::getGroundedState(bool p_grounded[exo_config::bnos::NB_LEG]) {
    float bufferAvg = 0.0f;
    updateBuffer(exo_config::bnos::LEFT_SHIN);

    for (size_t i = 0; i < exo_config::bnos::BUFFER_SIZE; ++i) 
    {
        bufferAvg += m_linAccelBufferLeft[i];
    }

    bufferAvg /= exo_config::bnos::BUFFER_SIZE;
    p_grounded[exo_config::bnos::LEFT_LEG] = bufferAvg >= exo_config::bnos::ACCEL_BUFFER_THRESHOLD;

    bufferAvg = 0.0f;
    updateBuffer(exo_config::bnos::RIGHT_SHIN);

    for (size_t i = 0; i < exo_config::bnos::BUFFER_SIZE; ++i) 
    {
        bufferAvg += m_linAccelBufferRight[i];
    }

    bufferAvg /= exo_config::bnos::BUFFER_SIZE;
    p_grounded[exo_config::bnos::RIGHT_LEG] = bufferAvg >= exo_config::bnos::ACCEL_BUFFER_THRESHOLD;
}

int16_t BnoHandler::getLinAccelYScaled(uint8_t p_position) 
{
    const float y = m_linearAccelerations[bnoIndex(p_position)].y;
    return static_cast<int16_t>(y * 256.0f);
}

void BnoHandler::printName(uint8_t p_position) 
{
    switch (p_position) 
    {
        case exo_config::bnos::LEFT_THIGH:
            Serial.print("THIGH_L");
            break;
        case exo_config::bnos::RIGHT_THIGH:
            Serial.print("THIGH_R");
            break;
        case exo_config::bnos::LEFT_SHIN:
            Serial.print("TIBIA_L");
            break;
        case exo_config::bnos::RIGHT_SHIN:
            Serial.print("TIBIA_R");
            break;
        case exo_config::bnos::EXO_BACK:
            Serial.print("EXO_BACK");
            break;
        case exo_config::bnos::MOBO:
            Serial.print("MOBO");
            break;
        default:
            Serial.print("Unknown");
            break;
    }
}

void BnoHandler::printBNOData(uint8_t p_position) 
{
    Serial.print("IMU ");
    printName(p_position);
    Serial.print("\n");
    Serial.print("Y: ");
    Serial.print(m_bnoAngles[bnoIndex(p_position)]);
    Serial.print("\n");
}

void BnoHandler::printBNOsStatus(int p_startIndex, int p_endIndex) 
{
    for (int i = p_startIndex; i <= p_endIndex; ++i) {
        Serial.print("\tIMU ");
        printName(i);
        Serial.print("\t");
        Serial.print("LINK: ");
        Serial.print(checkIfConnected(i));
    }
}

void BnoHandler::printBNOsData(int p_startIndex, int p_endIndex) 
{
    for (int i = p_startIndex; i <= p_endIndex; ++i) {
        printBNOData(i);
    }
}

void BnoHandler::printConnectedBNOsData(int p_startIndex, int p_endIndex) 
{
    for (int i = p_startIndex; i <= p_endIndex; ++i) {
        if (m_bnoConnected[i]) {
            printBNOData(i);
        }
    }
}