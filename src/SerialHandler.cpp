/**
 * @file SerialHandler.cpp
 * @brief Implementation of the SerialHandler class
 * 
 * @author Samuel Savaria
 * @date 2026-07-18
 */
#include "SerialHandler.hpp"
#include "config.hpp"

std::array<float, 4> SerialHandler::update()
{
    char command = Serial.read();
    while(command != -1)
    {
        switch(command)
        {
            case 'w': setTorque(m_torques[m_selectedMotorID] + 1);
                      break;
            case 's': setTorque(m_torques[m_selectedMotorID] - 1);
                      break;
            
            case 'd': setMotorID(m_selectedMotorID + 1);
                      break;
            case 'a': setMotorID(m_selectedMotorID - 1);
                      break;
            
            case 'r': restart();
                      break;
            
            default: break;
        }

        command = Serial.read();
    }

    return m_torques;
}

void SerialHandler::setTorque(float p_torque)
{
    m_torques[m_selectedMotorID] = p_torque;
    Serial.print("Motor #"); Serial.print(m_selectedMotorID);
    Serial.print(": "); Serial.print(p_torque); Serial.println(" Nm");
}

void SerialHandler::setMotorID(uint8_t p_motorID)
{
    m_selectedMotorID = (p_motorID + exo_config::motors::AMOUNT) % exo_config::motors::AMOUNT;
    Serial.print("Motor #"); Serial.print(m_selectedMotorID); Serial.println(" selected");
}

void SerialHandler::restart()
{
    ESP.restart();
}