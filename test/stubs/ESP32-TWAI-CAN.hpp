/**
 * @file ESP32-TWAI-CAN.hpp
 * @brief Native test stub for the ESP32-TWAI-CAN library
 *
 * @details Mirrors the CanFrame layout and ESP32Can API surface actually used by the production
 *          code (see motorHandler.cpp, CubemarsMotorV2.cpp, CubemarsMotorV3.cpp, main.cpp) so
 *          modules compile natively (env:native, see platformio.ini). Tests control it directly:
 *          push frames onto ESP32Can.rxQueue for the code under test to receive, and inspect
 *          ESP32Can.txHistory for what was sent. Call ESP32Can.reset() between tests.
 */
#ifndef TEST_STUBS_ESP32_TWAI_CAN_HPP
#define TEST_STUBS_ESP32_TWAI_CAN_HPP

#include <cstdint>
#include <queue>
#include <vector>

/**
 * @brief Mirrors the public layout of the real CanFrame (twai_message_t)
 */
struct CanFrame
{
    uint32_t extd : 1;
    uint32_t rtr : 1;
    uint32_t ss : 1;
    uint32_t self : 1;
    uint32_t dlc_non_comp : 1;
    uint32_t reserved : 27;
    uint32_t identifier;
    uint8_t data_length_code;
    uint8_t data[8];
};

/**
 * @brief Fake replacement for the ESP32Can global object
 */
class ESP32TWAICanFake
{
public:
    bool begin(int = 0, int8_t = -1, int8_t = -1, uint16_t = 0, uint16_t = 0) { return true; }
    int convertSpeed(int kbps) { return kbps; }

    bool writeFrame(const CanFrame& frame, uint32_t = 1)
    {
        txHistory.push_back(frame);
        return true;
    }

    bool readFrame(CanFrame* frame, uint32_t = 1000)
    {
        if(rxQueue.empty()) return false;
        *frame = rxQueue.front();
        rxQueue.pop();
        return true;
    }

    bool readFrame(CanFrame& frame, uint32_t timeout = 1000)
    {
        return readFrame(&frame, timeout);
    }

    /// Clears pending rx frames and sent-frame history. Call between tests.
    void reset()
    {
        while(!rxQueue.empty())
        {
            rxQueue.pop();
        }
        txHistory.clear();
    }

    std::queue<CanFrame> rxQueue;
    std::vector<CanFrame> txHistory;
};

static ESP32TWAICanFake ESP32Can;

#endif
