/**
 * @file UserMorphology.hpp
 * @brief Declaration of the UserMorphology singleton
 * 
 * @author Gabriel Desrochers, Éloi Charbonneau
 * @date 2026-09-30
 */

#ifndef USER_MORPHOLOGY_HPP
#define USER_MORPHOLOGY_HPP

#include <Arduino.h>
#include "Config.hpp"

/**
 * @brief Holds the user morphology shared by every context.
 *
 * @details The values are written by the HMI task and read by the control task,
 *          so every access is protected by a mutex.
 */
class UserMorphology
{
public:
    /**
     * @brief Get the single instance of the class.
     *
     * @return Reference to the UserMorphology instance.
     */
    static UserMorphology& getInstance();

    UserMorphology(const UserMorphology&) = delete;
    UserMorphology& operator=(const UserMorphology&) = delete;

    /**
     * @brief Set the user morphology and update the derived segment lengths and forces.
     *
     * @param[in] p_height User height in centimeters.
     * @param[in] p_mass User mass in kilograms.
     */
    void setMorphology(int p_height, int p_mass);

    /**
     * @brief Get the user height.
     *
     * @return User height in meters.
     */
    float getHeight();

    /**
     * @brief Get the user mass.
     *
     * @return User mass in kilograms.
     */
    float getMass();

    /**
     * @brief Get the torso length.
     *
     * @return Torso length in meters.
     */
    float getLengthTorso();

    /**
     * @brief Get the thigh length.
     *
     * @return Thigh length in meters.
     */
    float getLengthThigh();

    /**
     * @brief Get the calf length.
     *
     * @return Calf length in meters.
     */
    float getLengthCalf();

    /**
     * @brief Get the force applied on the torso, including the exoskeleton weight.
     *
     * @return Torso force in Newtons.
     */
    float getForceTorso();

    /**
     * @brief Get the force applied on a thigh.
     *
     * @return Thigh force in Newtons.
     */
    float getForceThigh();

    /**
     * @brief Get the force applied on a calf.
     *
     * @return Calf force in Newtons.
     */
    float getForceCalf();

private:
    /**
     * @brief Construct the UserMorphology with the default morphology.
     */
    UserMorphology();

    /**
     * @brief Read a morphology value while holding the mutex.
     *
     * @param[in] p_value Reference to the value to read.
     * @return Copy of the value.
     */
    float readValue(const float& p_value);

    SemaphoreHandle_t m_mutex; ///< Protects the morphology values from concurrent access.
    float m_height;       ///< User height [m].
    float m_mass;         ///< User mass [kg].
    float m_lengthTorso;  ///< Length of the torso segment [m].
    float m_lengthThigh;  ///< Length of the thigh segment [m].
    float m_lengthCalf;   ///< Length of the calf segment [m].
    float m_forceTorso;   ///< Force applied on the torso segment [N].
    float m_forceThigh;   ///< Force applied on the thigh segment [N].
    float m_forceCalf;    ///< Force applied on the calf segment [N].
};

#endif
