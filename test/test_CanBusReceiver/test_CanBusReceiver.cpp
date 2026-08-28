/**
 * @file test_CanBus_receiver.cpp
 * @brief Native compile + behavior smoke tests for CanBusReceiver
 *
 * @details Reference example for testing the other src/ modules natively:
 *          - Source files are included directly (test_build_src = false, see platformio.ini)
 *          - ESP32Can is faked (see test/stubs/ESP32-TWAI-CAN.hpp): frames are pushed onto
 *            ESP32Can.rxQueue for update() to read, and ESP32Can.reset() clears that queue
 *            between tests.
 *          - Each test suite owns its own Unity main(), there is no shared runner.
 */
#include <unity.h>
#include <ESP32-TWAI-CAN.hpp>

#include "../../src/CanBusReceiver.cpp"

namespace
{
    CanFrame makeFrame(uint32_t identifier)
    {
        CanFrame frame{};
        frame.identifier = identifier;
        frame.data_length_code = 0;
        return frame;
    }
}

void setUp(void)
{
    ESP32Can.reset();
}

void tearDown(void) {}

void test_update_withNoFrames_doesNotInvokeCallback(void)
{
    CanBusReceiver receiver;
    int callCount = 0;
    receiver.subscribe([&](const CanFrame&) { ++callCount; }, false, 0);

    receiver.update();

    TEST_ASSERT_EQUAL_INT(0, callCount);
}

void test_update_dispatchesFrameToSubscriber(void)
{
    CanBusReceiver receiver;
    uint32_t receivedId = 0;
    int callCount = 0;
    receiver.subscribe([&](const CanFrame& frame)
    {
        ++callCount;
        receivedId = frame.identifier;
    }, false, 0);
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(1, callCount);
    TEST_ASSERT_EQUAL_UINT32(0x123, receivedId);
}

void test_update_dispatchesFrameToEverySubscriber(void)
{
    CanBusReceiver receiver;
    int firstCallCount = 0;
    int secondCallCount = 0;
    receiver.subscribe([&](const CanFrame&) { ++firstCallCount; }, false, 0);
    receiver.subscribe([&](const CanFrame&) { ++secondCallCount; }, false, 0);
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(1, firstCallCount);
    TEST_ASSERT_EQUAL_INT(1, secondCallCount);
}

void test_update_drainsEveryPendingFrame(void)
{
    CanBusReceiver receiver;
    std::vector<uint32_t> receivedIds;
    receiver.subscribe([&](const CanFrame& frame) { receivedIds.push_back(frame.identifier); }, false, 0);
    ESP32Can.rxQueue.push(makeFrame(0x111));
    ESP32Can.rxQueue.push(makeFrame(0x222));
    ESP32Can.rxQueue.push(makeFrame(0x333));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(3, receivedIds.size());
    TEST_ASSERT_EQUAL_UINT32(0x111, receivedIds[0]);
    TEST_ASSERT_EQUAL_UINT32(0x222, receivedIds[1]);
    TEST_ASSERT_EQUAL_UINT32(0x333, receivedIds[2]);
}

void test_update_withoutAnySubscriber_doesNotCrash(void)
{
    CanBusReceiver receiver;
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_PASS();
}

void test_update_identifierFiltered_onlyFiresForMatchingId(void)
{
    CanBusReceiver receiver;
    int matchingCallCount = 0;
    int otherCallCount = 0;
    receiver.subscribe([&](const CanFrame&) { ++matchingCallCount; }, true, 0x123);
    receiver.subscribe([&](const CanFrame&) { ++otherCallCount; }, true, 0x456);
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(1, matchingCallCount);
    TEST_ASSERT_EQUAL_INT(0, otherCallCount);
}

void test_subscribe_upToArraySize_stillDispatchesToEverySubscriber(void)
{
    CanBusReceiver receiver;
    int callCount = 0;
    for(size_t i = 0; i < CanBusReceiver::MAX_SUBSCRIBERS; i++)
    {
        receiver.subscribe([&](const CanFrame&) { ++callCount; }, false, 0);
    }
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(CanBusReceiver::MAX_SUBSCRIBERS, callCount);
}


int main(int argc, char** argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_update_withNoFrames_doesNotInvokeCallback);
    RUN_TEST(test_update_dispatchesFrameToSubscriber);
    RUN_TEST(test_update_dispatchesFrameToEverySubscriber);
    RUN_TEST(test_update_drainsEveryPendingFrame);
    RUN_TEST(test_update_withoutAnySubscriber_doesNotCrash);
    RUN_TEST(test_update_identifierFiltered_onlyFiresForMatchingId);
    RUN_TEST(test_subscribe_upToArraySize_stillDispatchesToEverySubscriber);
    return UNITY_END();
}
