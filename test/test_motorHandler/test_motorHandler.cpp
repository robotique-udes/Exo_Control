/**
 * @file test_motorHandler.cpp
 * @brief Native compile + behavior smoke tests for MotorHandler
 *
 * @details Reference example for testing the other src/ modules natively:
 *          - Source files are included directly (test_build_src = false, see platformio.ini)
 *          - Arduino core calls (millis, constrain, ...) are faked with ArduinoFake/FakeIt
 *          - MotorHandler takes its motors via constructor injection, so tests give it a
 *            FakeMotor (a minimal ICubemarsMotor implementation) instead of real CubemarsMotorV2/V3
 *            objects. Torque is read directly from the FakeMotor, and temperature/error state is set
 *            directly on it, so tests don't need to build/decode real CAN payloads. MotorHandler still
 *            drains ESP32Can internally (see test/stubs/ESP32-TWAI-CAN.hpp), it's just left empty here.
 *          - Each test suite owns its own Unity main(), there is no shared runner.
 */
#include <unity.h>
#include <ArduinoFake.h>
#include <FakeMotor.hpp>

#include "../../src/utils/movingAverage.cpp"

// Each source file declares its own file-scoped "static const bool DEBUG_PRINT" (see printMacro.hpp).
// Renamed via macro substitution so both definitions can coexist in this single translation unit.
#define DEBUG_PRINT test_ICubemarsMotor_DEBUG_PRINT
#include "../../src/ICubemarsMotor.cpp"
#undef DEBUG_PRINT

#define DEBUG_PRINT test_motorHandler_DEBUG_PRINT
#include "../../src/motorHandler.cpp"
#undef DEBUG_PRINT



namespace
{
    FakeMotor kneeLeft(exo_config::motors::KNEE_LEFT);
    FakeMotor kneeRight(exo_config::motors::KNEE_RIGHT);
    FakeMotor hipLeft(exo_config::motors::HIP_LEFT);
    FakeMotor hipRight(exo_config::motors::HIP_RIGHT);

    uint32_t fakeMillis = 0;

    MotorHandler makeMotorHandler()
    {
        return MotorHandler({&kneeLeft, &kneeRight, &hipLeft, &hipRight});
    }
}

void setUp(void)
{
    ArduinoFakeReset();
    fakeMillis = 0;
    fakeit::When(Method(ArduinoFake(), millis)).AlwaysDo([]() { return fakeMillis++; }); 

    for(FakeMotor* motor : {&kneeLeft, &kneeRight, &hipLeft, &hipRight})
    {
        motor->enterModeCalls = 0;
        motor->sendCommandCalls = 0;
        motor->lastTorque = 0.0f;
        motor->setTemperature(0);
        motor->setErrorCode(CubemarsErrorCode::NO_FAULT);
    }
}

void tearDown(void) {}

void test_construction_doesNotCommandMotors(void)
{
    MotorHandler motorHandler = makeMotorHandler();

    TEST_ASSERT_EQUAL_INT(0, kneeLeft.sendCommandCalls);
}

void test_disabledByDefault_forcesZeroTorque(void)
{
    MotorHandler motorHandler = makeMotorHandler();

    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    TEST_ASSERT_EQUAL_FLOAT(0.0f, kneeLeft.lastTorque);
}

void test_enableMotors_entersModeOnEveryMotor(void)
{
    MotorHandler motorHandler = makeMotorHandler();

    motorHandler.enableMotors();

    TEST_ASSERT_EQUAL_INT(1, kneeLeft.enterModeCalls);
    TEST_ASSERT_EQUAL_INT(1, kneeRight.enterModeCalls);
    TEST_ASSERT_EQUAL_INT(1, hipLeft.enterModeCalls);
    TEST_ASSERT_EQUAL_INT(1, hipRight.enterModeCalls);
}

void test_overTemperatureMotor_tripsSafetyAndZeroesTorqueOnEveryMotor(void)
{
    MotorHandler motorHandler = makeMotorHandler();
    motorHandler.enableMotors();
    kneeLeft.setTemperature(90); // Above exo_config::motors::MAX_TEMPERATURE

    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    TEST_ASSERT_EQUAL_FLOAT(0.0f, kneeLeft.lastTorque);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, hipRight.lastTorque);
}

void test_motorError_tripsSafetyAndZeroesTorque(void)
{
    MotorHandler motorHandler = makeMotorHandler();
    motorHandler.enableMotors();
    hipLeft.setErrorCode(CubemarsErrorCode::ENCODER_FAULT);

    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    TEST_ASSERT_EQUAL_FLOAT(0.0f, hipLeft.lastTorque);
}

void test_enabledMotor_appliesNonZeroTorque(void)
{
    MotorHandler motorHandler = makeMotorHandler();
    motorHandler.enableMotors();

    // The moving average returns 0 on its very first sample, so a second update is needed to observe the applied torque
    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});
    motorHandler.update({5.0f, 5.0f, 5.0f, 5.0f});

    TEST_ASSERT_GREATER_THAN_FLOAT(0.0f, kneeLeft.lastTorque);
}

int main(int argc, char** argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_construction_doesNotCommandMotors);
    RUN_TEST(test_disabledByDefault_forcesZeroTorque);
    RUN_TEST(test_enableMotors_entersModeOnEveryMotor);
    RUN_TEST(test_overTemperatureMotor_tripsSafetyAndZeroesTorqueOnEveryMotor);
    RUN_TEST(test_motorError_tripsSafetyAndZeroesTorque);
    RUN_TEST(test_enabledMotor_appliesNonZeroTorque);
    return UNITY_END();
}
