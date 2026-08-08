/**
 * @file test_motorHandler.cpp
 * @brief Native compile + behavior smoke tests for MotorHandler
 *
 * @details Reference example for testing the other src/ modules natively:
 *          - Source files are included directly (test_build_src = false, see platformio.ini)
 *          - Arduino core calls (millis, constrain, ...) are faked with ArduinoFake/FakeIt
 *          - Hardware-only dependencies (ESP32-TWAI-CAN) are faked in test/stubs and controlled
 *            directly from the test: push CanFrames onto ESP32Can.rxQueue to simulate what the
 *            motor reports, and inspect ESP32Can.txHistory to see what was sent to the motor.
 *          - Each test suite owns its own Unity main(), there is no shared runner.
 */
#include <unity.h>
#include <ArduinoFake.h>

#include "../../src/utils/movingAverage.cpp"
#include "../../src/ICubemarsMotor.cpp"
#include "../../src/CubemarsMotorV2.cpp"
#include "../../src/CubemarsMotorV3.cpp"
#include "../../src/motorHandler.cpp"

using namespace fakeit;

namespace
{
    // Mirrors CubemarsMotorV2's private torque range/bit width (see CubemarsMotorV2.hpp) so the
    // CAN payloads it sends can be decoded back into a torque value from the test.
    constexpr float TORQUE_MIN = -65.0f;
    constexpr float TORQUE_MAX = 65.0f;
    constexpr uint8_t TORQUE_BITS = 12;

    float decodeTorque(const CanFrame& frame)
    {
        uint16_t raw = ((frame.data[6] & 0x0F) << 8) | frame.data[7];
        float span = TORQUE_MAX - TORQUE_MIN;
        return raw * span / ((1 << TORQUE_BITS) - 1) + TORQUE_MIN;
    }

    // Returns the most recent frame sent for the given motor identifier
    const CanFrame& findLastFrame(uint32_t identifier)
    {
        for(auto it = ESP32Can.txHistory.rbegin(); it != ESP32Can.txHistory.rend(); ++it)
        {
            if(it->identifier == identifier)
            {
                return *it;
            }
        }

        TEST_FAIL_MESSAGE("No CAN frame found for the requested motor identifier");
        static CanFrame empty{};
        return empty;
    }
}

void setUp(void)
{
    ArduinoFakeReset();
    When(Method(ArduinoFake(), millis)).AlwaysReturn(0);
    ESP32Can.reset();
}

void tearDown(void) {}

void test_construction_sendsNoCanTraffic(void)
{
    MotorHandler motorHandler;

    TEST_ASSERT_TRUE(ESP32Can.txHistory.empty());
}

void test_disabledByDefault_forcesZeroTorque(void)
{
    MotorHandler motorHandler;

    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    const CanFrame& frame = findLastFrame(exo_config::motors::KNEE_LEFT);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.0f, decodeTorque(frame));
}

void test_enableMotors_sendsModeSetupFrames(void)
{
    MotorHandler motorHandler;

    motorHandler.enableMotors();

    TEST_ASSERT_FALSE(ESP32Can.txHistory.empty());
}

void test_overTemperatureReading_tripsSafetyAndZeroesTorque(void)
{
    MotorHandler motorHandler;
    motorHandler.enableMotors();
    ESP32Can.reset(); // Discard the mode-setup frames, keep only what update() sends below

    // Simulate the left knee motor reporting an over-temperature reading over CAN
    CanFrame overTempReply{};
    overTempReply.identifier = exo_config::motors::KNEE_LEFT;
    overTempReply.data_length_code = 8;
    overTempReply.data[6] = 150; // temperature = data[6] - 40 = 110 degC, above MAX_TEMPERATURE
    ESP32Can.rxQueue.push(overTempReply);

    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    const CanFrame& frame = findLastFrame(exo_config::motors::KNEE_LEFT);
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.0f, decodeTorque(frame));
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_construction_sendsNoCanTraffic);
    RUN_TEST(test_disabledByDefault_forcesZeroTorque);
    RUN_TEST(test_enableMotors_sendsModeSetupFrames);
    RUN_TEST(test_overTemperatureReading_tripsSafetyAndZeroesTorque);
    return UNITY_END();
}
