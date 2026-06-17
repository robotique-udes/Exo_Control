/**
 * @file CubemarsAK10-9KV100V2.h
 * @brief Declaration of the Cubemars_AK10_9_KV100_V2 implementation class
 * @see Datasheet: https://www.cubemars.com/data/cms/202604/ak-series-driver-manual-v1-0-18-for-ak-2-0-robotic-actuator.pdf
 * 
 * @author Samuel Savaria
 * @date 2026-06-07
 */
#ifndef BIOGENIUS_CUBEMARS_AK10_9_KV100_V2_H
#define BIOGENIUS_CUBEMARS_AK10_9_KV100_V2_H

#include "MitModeMotor.h"

enum CubemarsV2ErrorCode : uint8_t
{
    FAULT_CODE_NONE = 0, 
    FAULT_CODE_OVER_VOLTAGE = 1,
    FAULT_CODE_UNDER_VOLTAGE = 2,
    FAULT_CODE_DRV = 3,
    FAULT_CODE_ABS_OVER_CURRENT = 4,
    FAULT_CODE_OVER_TEMP_FET = 5,
    FAULT_CODE_OVER_TEMP_MOTOR = 6,
    FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE = 7,
    FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE = 8,
    FAULT_CODE_MCU_UNDER_VOLTAGE = 9,
    FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET = 10,
    FAULT_CODE_ENCODER_SPI = 11,
    FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE = 12,
    FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE = 13,
    FAULT_CODE_FLASH_CORRUPTION = 14,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1 = 15,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2 = 16,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3 = 17,
    FAULT_CODE_UNBALANCED_CURRENTS = 18
};

/**
 * @brief Implementation of the IMitModeMotor interface for a CubeMars AK10-9 KV100 V2 motor
 *
 * @details The class contains the parameter range for the AK10-9 KV100 V2 model
 *          and implements the interface's pure virtual functions
 *
 * @author Samuel Savaria
 * @date 2026-06-07
 */
class Cubemars_AK10_9_KV100_V2 : public IMitModeMotor
{
public:
/**
     * @brief Constructor
     *
     * @param[in] id The 8-bit CAN ID of the motor
     */
    Cubemars_AK10_9_KV100_V2(uint8_t id);

    /**
     * @brief Enter MIT mode
     */
    void enterMode() override;

    /**
     * @brief Sends a MIT mode command
     *
     * @param[in] position The target position in radians. Also requires KP and KD
     * @param[in] velocity The target velocity in radians/second. Also requires KD
     * @param[in] torque   The target torque in newtons-meter.
     * @param[in] kp       The proportional gain. Required by position
     * @param[in] kd       The derivative gain. Required by position and velocity
     */
    void sendCommand(float position, float velocity, float torque, float kp, float kd) override;

    /**
     * @brief Parses a CAN message sent by the motor containing the position, speed, torque, temperature and error code
     *
     * @param[in] message The CAN message to parse
     */
    void receiveCommand(const CanFrame& message) override;

    /**
     * @brief Returns an error description corresponding to the error code
     * 
     * @return The error description
     */
    const char* getErrorDescription() const override;
    
private:
    static constexpr float POSITION_MIN = -12.5f;
    static constexpr float POSITION_MAX =  12.5f;
    static constexpr float VELOCITY_MIN = -50.0f;
    static constexpr float VELOCITY_MAX =  50.0f;
    static constexpr float TORQUE_MIN   = -65.0f;
    static constexpr float TORQUE_MAX   =  65.0f;
    static constexpr float KP_MIN       =  0.0f;
    static constexpr float KP_MAX       =  500.0f;
    static constexpr float KD_MIN       =  0.0f;
    static constexpr float KD_MAX       =  5.0f;
    static constexpr uint8_t ERROR_COUNT = 19; // The number of possible errors
    static constexpr const char* ERROR_DESCRIPTIONS[ERROR_COUNT] = {
    "None",                          // FAULT_CODE_NONE
    "OverVoltage",                   // FAULT_CODE_OVER_VOLTAGE
    "UnderVoltage",                  // FAULT_CODE_UNDER_VOLTAGE
    "DriverFault",                   // FAULT_CODE_DRV
    "AbsOverCurrent",                // FAULT_CODE_ABS_OVER_CURRENT
    "OverTemperatureFet",            // FAULT_CODE_OVER_TEMP_FET
    "OverTemperatureMotor",          // FAULT_CODE_OVER_TEMP_MOTOR
    "GateDriverOverVoltage",         // FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE
    "GateDriverUnderVoltage",        // FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE
    "McuUnderVoltage",               // FAULT_CODE_MCU_UNDER_VOLTAGE
    "BootingFromWatchdogReset",      // FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET
    "EncoderSpi",                    // FAULT_CODE_ENCODER_SPI
    "EncoderSincosBelowMinAmplitude",// FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE
    "EncoderSincosAboveMaxAmplitude",// FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE
    "FlashCorruption",               // FAULT_CODE_FLASH_CORRUPTION
    "HighOffsetCurrentSensor1",      // FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1
    "HighOffsetCurrentSensor2",      // FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2
    "HighOffsetCurrentSensor3",      // FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3
    "UnbalancedCurrents"             // FAULT_CODE_UNBALANCED_CURRENTS
    };
};

#endif