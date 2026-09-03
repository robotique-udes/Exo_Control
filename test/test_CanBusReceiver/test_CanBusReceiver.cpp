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
#include <vector>

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

    class RecordingObserver : public ICanObserver
    {
    public:
        void notify(const CanFrame& p_frame) override
        {
            ++callCount;
            lastId = p_frame.identifier;
            receivedIds.push_back(p_frame.identifier);
        }

        int callCount = 0;
        uint32_t lastId = 0;
        std::vector<uint32_t> receivedIds;
    };
}

void setUp(void)
{
    ESP32Can.reset();
}

void tearDown(void) {}

void test_update_withNoFrames_doesNotInvokeObserver(void)
{
    CanBusReceiver receiver;
    RecordingObserver observer;
    receiver.addObserver(&observer);

    receiver.update();

    TEST_ASSERT_EQUAL_INT(0, observer.callCount);
}

void test_update_dispatchesFrameToObserver(void)
{
    CanBusReceiver receiver;
    RecordingObserver observer;
    receiver.addObserver(&observer);
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(1, observer.callCount);
    TEST_ASSERT_EQUAL_UINT32(0x123, observer.lastId);
}

void test_update_dispatchesFrameToEveryObserver(void)
{
    CanBusReceiver receiver;
    RecordingObserver firstObserver;
    RecordingObserver secondObserver;
    receiver.addObserver(&firstObserver);
    receiver.addObserver(&secondObserver);
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(1, firstObserver.callCount);
    TEST_ASSERT_EQUAL_INT(1, secondObserver.callCount);
}

void test_update_drainsEveryPendingFrame(void)
{
    CanBusReceiver receiver;
    RecordingObserver observer;
    receiver.addObserver(&observer);
    ESP32Can.rxQueue.push(makeFrame(0x111));
    ESP32Can.rxQueue.push(makeFrame(0x222));
    ESP32Can.rxQueue.push(makeFrame(0x333));

    receiver.update();

    TEST_ASSERT_EQUAL_INT(3, observer.receivedIds.size());
    TEST_ASSERT_EQUAL_UINT32(0x111, observer.receivedIds[0]);
    TEST_ASSERT_EQUAL_UINT32(0x222, observer.receivedIds[1]);
    TEST_ASSERT_EQUAL_UINT32(0x333, observer.receivedIds[2]);
}

void test_update_withoutAnyObserver_doesNotCrash(void)
{
    CanBusReceiver receiver;
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    TEST_PASS();
}

void test_addObserver_upToArraySize_stillDispatchesToEveryObserver(void)
{
    CanBusReceiver receiver;
    std::vector<RecordingObserver> observers(CanBusReceiver::MAX_SUBSCRIBERS);
    for(size_t i = 0; i < CanBusReceiver::MAX_SUBSCRIBERS; i++)
    {
        receiver.addObserver(&observers[i]);
    }
    ESP32Can.rxQueue.push(makeFrame(0x123));

    receiver.update();

    for(size_t i = 0; i < CanBusReceiver::MAX_SUBSCRIBERS; i++)
    {
        TEST_ASSERT_EQUAL_INT(1, observers[i].callCount);
    }
}


int main(int argc, char** argv)
{
    UNITY_BEGIN();
    RUN_TEST(test_update_withNoFrames_doesNotInvokeObserver);
    RUN_TEST(test_update_dispatchesFrameToObserver);
    RUN_TEST(test_update_dispatchesFrameToEveryObserver);
    RUN_TEST(test_update_drainsEveryPendingFrame);
    RUN_TEST(test_update_withoutAnyObserver_doesNotCrash);
    RUN_TEST(test_addObserver_upToArraySize_stillDispatchesToEveryObserver);
    return UNITY_END();
}
