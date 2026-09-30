/**
 * @file UserMorphology.cpp
 * @brief Implementation of the UserMorphology singleton
 * 
 * @author Gabriel Desrochers, Éloi Charbonneau
 * @date 2026-09-30
 */

#include "UserMorphology.hpp"

static constexpr int DEFAULT_HEIGHT_CM = 170;
static constexpr int DEFAULT_MASS_KG = 70;

UserMorphology::UserMorphology()
{
    m_mutex = xSemaphoreCreateMutex();
    setMorphology(DEFAULT_HEIGHT_CM, DEFAULT_MASS_KG);
}

UserMorphology& UserMorphology::getInstance()
{
    static UserMorphology instance;
    return instance;
}

void UserMorphology::setMorphology(int p_height, int p_mass)
{
    if (xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE)
    {
        m_height = p_height / 100.0f;
        m_mass = p_mass;

        float gravitationalForce = m_mass * exo_config::physics::GRAVITY;
        float exoForce = exo_config::physics::EXO_MASS * exo_config::physics::GRAVITY;

        m_lengthTorso = exo_config::anatomy::PROPORTION_TORSO_LENGTH * m_height;
        m_lengthThigh = exo_config::anatomy::PROPORTION_THIGH_LENGTH * m_height;
        m_lengthCalf = exo_config::anatomy::PROPORTION_CALF_LENGTH * m_height;

        m_forceTorso = exo_config::anatomy::PROPORTION_TORSO_MASS * gravitationalForce + exoForce;
        m_forceThigh = exo_config::anatomy::PROPORTION_THIGH_MASS * gravitationalForce;
        m_forceCalf = exo_config::anatomy::PROPORTION_CALF_MASS * gravitationalForce;

        xSemaphoreGive(m_mutex);
    }
}

float UserMorphology::readValue(const float& p_value)
{
    float value = 0.0f;
    if (xSemaphoreTake(m_mutex, portMAX_DELAY) == pdTRUE)
    {
        value = p_value;
        xSemaphoreGive(m_mutex);
    }
    return value;
}

float UserMorphology::getHeight()
{
    return readValue(m_height);
}

float UserMorphology::getMass()
{
    return readValue(m_mass);
}

float UserMorphology::getLengthTorso()
{
    return readValue(m_lengthTorso);
}

float UserMorphology::getLengthThigh()
{
    return readValue(m_lengthThigh);
}

float UserMorphology::getLengthCalf()
{
    return readValue(m_lengthCalf);
}

float UserMorphology::getForceTorso()
{
    return readValue(m_forceTorso);
}

float UserMorphology::getForceThigh()
{
    return readValue(m_forceThigh);
}

float UserMorphology::getForceCalf()
{
    return readValue(m_forceCalf);
}
