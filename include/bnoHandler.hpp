 /** 
 * @file BNOHandler.hpp
 * @author Eloi Charbonneau
 * @date 2026-07-18
 */

#ifndef BNOHANDLER_HPP
#define BNOHANDLER_HPP

#include "SparkFun_BNO080_Arduino_Library.h"
#include "multiplex.hpp"
#include "Config.hpp"
#include <array>

/**
 * @brief Simple 3-axis linear-acceleration container.
 */
typedef struct
{
    float x;
    float y;
    float z;
} linearAcceleration_t;


/**
 * @brief Manager for multiple BNO080 devices connected through an I2C multiplexer.
 *
 * @details The class maintains BNO080 objects, connection state, recent linear-acceleration
 *          samples and computed joint angles. It provides methods to initialize devices,
 *          poll sensor data and query processed outputs.
 */
class BnoHandler {
public:
    // --- 1. PUBLIC METHODS ---

    /**
     * @brief Construct the BNO handler and initialize the internal buffers.
     */
    BnoHandler();
    
    /**
     * @brief Poll all connected BNOs for available reports and update
     * internal `angles` and `linearAccelerations` buffers.
     */
    void requestData();

    /**
     * @brief Return the computed joint angles.
     *
     * @param[out] p_angles Output array filled with the current joint angles in degrees.
     */
    void getAngle(float p_angles[exo_config::bnos::AMOUNT]);

    /**
     * @brief Compute and return the left and right grounded state.
     *
     * @param[out] p_grounded Output array filled with the grounded state for each leg.
     */
    void getGroundedState(bool p_grounded[exo_config::bnos::NB_LEG]);

    /**
     * @brief Initialize all BNO devices by selecting the mux channel and calling begin.
     *
     * @return true if at least one device was successfully initialized, false otherwise.
     */
    bool begin();

    /**
     * @brief Print the name and connection status of all BNOs.
     *
     * @param[in] p_startIndex Index of the first BNO to print.
     * @param[in] p_endIndex Index of the last BNO to print.
     */
    void printBNOsStatus(int p_startIndex = 0, int p_endIndex = 4);

    /**
     * @brief Print accelerometer and linear-acceleration values for a BNO range.
     *
     * @param[in] p_startIndex Index of the first BNO to print.
     * @param[in] p_endIndex Index of the last BNO to print.
     */
    void printBNOsData(int p_startIndex = 0, int p_endIndex = 4);

    /**
     * @brief Print sensor values only for BNOs that are currently connected.
     *
     * @param[in] p_startIndex Index of the first BNO to print.
     * @param[in] p_endIndex Index of the last BNO to print.
     */
    void printConnectedBNOsData(int p_startIndex = 0, int p_endIndex = 4);

    /**
     * @brief Print a human-readable name for one BNO position.
     *
     * @param[in] p_position BNO position to print.
     */
    void printName(uint8_t p_position);

    /**
     * @brief Print accelerometer and linear-acceleration data for one BNO.
     *
     * @param[in] p_position BNO position to print.
     */
    void printBNOData(uint8_t p_position);

private:
    // --- 2. PRIVATE METHODS ---

    /**
     * @brief Convert a BNO position enum value to an array index.
     *
     * @param[in] p_position BNO position value.
     * @return Zero-based index matching the internal storage order.
     */
    static constexpr size_t bnoIndex(uint8_t p_position) {
        return static_cast<size_t>(p_position);
    }

    /**
     * @brief Write the latest linear-acceleration Y value into the circular buffer.
     *
     * @param[in] p_position BNO position whose buffer must be updated.
     */
    void updateBuffer(uint8_t p_position);

    /**
     * @brief Configure the reports required from the BNO080 at the given position.
     *
     * @param[in] p_position BNO position to configure.
     */
    void setupReports(uint8_t p_position);

    /**
     * @brief Probe a BNO device by selecting the mux channel and attempting an I2C access.
     *
     * @param[in] p_position BNO position to probe.
     * @return true if the device acknowledged on I2C, false otherwise.
     */
    bool checkIfConnected(uint8_t p_position);

    /**
     * @brief Read the buffered linear-acceleration Y value and scale it to Q8 format.
     *
     * @param[in] p_position BNO position to read from.
     * @return Scaled Y linear-acceleration value in Q8 format.
     */
    int16_t getLinAccelYScaled(uint8_t p_position);

    // --- 3. PRIVATE ATTRIBUTES ---

    std::array<BNO080, exo_config::bnos::AMOUNT> m_bnoDevices; ///< Array of physical BNO08x instances, ordered by position.
    std::array<bool, exo_config::bnos::AMOUNT> m_bnoConnected; ///< Connection state for every BNO.
    std::array<uint8_t, exo_config::bnos::AMOUNT> m_muxChannels; ///< Mux channel used for each BNO.
    std::array<uint8_t, exo_config::bnos::AMOUNT> m_i2cAddresses; ///< I2C address used for each BNO.
    std::array<linearAcceleration_t, exo_config::bnos::AMOUNT> m_linearAccelerations; ///< Linear-acceleration values for each BNO.
    std::array<float, exo_config::bnos::AMOUNT> m_bnoAngles; ///< Angle output for every BNO.
    Multiplex m_mux; ///< Multiplexer used to switch between BNO sensors.

    long m_lastUpdate = 0; ///< Time of the last update, based on millis().
    int m_bufferIndexLeft;
    float m_linAccelBufferLeft[exo_config::bnos::BUFFER_SIZE];
    int m_bufferIndexRight;
    float m_linAccelBufferRight[exo_config::bnos::BUFFER_SIZE];
    int m_offset = 0;
};
#endif