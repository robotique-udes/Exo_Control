/**
 * @file test_ContextRunner.cpp
 * @brief Native tests for ContextRunner context selection
 */
#include <unity.h>

#include "../../src/ContextRunner.cpp"

namespace
{
    /// Stub context returning a fixed validity and a recognizable torque.
    class StubContext : public IContext
    {
    public:
        StubContext(float p_validity, float p_torqueValue)
            : m_validity(p_validity), m_torqueValue(p_torqueValue) {}

        float computeContextValidity(const float p_angles[exo_config::bnos::AMOUNT]) override
        {
            (void)p_angles;
            validityCalls++;
            return m_validity;
        }

        void computeTorque(const float p_angles[exo_config::bnos::AMOUNT],
                           const bool p_grounded[exo_config::bnos::NB_LEG],
                           float (&p_torque)[exo_config::motors::AMOUNT]) override
        {
            (void)p_angles;
            (void)p_grounded;
            torqueCalls++;
            for (uint8_t i = 0; i < exo_config::motors::AMOUNT; ++i)
            {
                p_torque[i] = m_torqueValue;
            }
        }

        int validityCalls = 0;
        int torqueCalls = 0;

    private:
        float m_validity;
        float m_torqueValue;
    };

    float angles[exo_config::bnos::AMOUNT] = {0};
    bool grounded[exo_config::bnos::NB_LEG] = {false};
}

void setUp() {}
void tearDown() {}

void test_selects_highest_validity()
{
    StubContext low(0.2f, 1.0f);
    StubContext high(0.9f, 2.0f);
    StubContext mid(0.5f, 3.0f);
    ContextRunner runner;
    runner.registerContext(&low);
    runner.registerContext(&high);
    runner.registerContext(&mid);

    float torque[exo_config::motors::AMOUNT] = {0};
    runner.update(angles, grounded, torque);

    TEST_ASSERT_EQUAL_INT(0, low.torqueCalls);
    TEST_ASSERT_EQUAL_INT(1, high.torqueCalls);
    TEST_ASSERT_EQUAL_INT(0, mid.torqueCalls);
    for (uint8_t i = 0; i < exo_config::motors::AMOUNT; ++i)
    {
        TEST_ASSERT_EQUAL_FLOAT(2.0f, torque[i]);
    }
}

void test_all_contexts_compute_validity()
{
    StubContext a(0.1f, 1.0f);
    StubContext b(0.8f, 2.0f);
    ContextRunner runner;
    runner.registerContext(&a);
    runner.registerContext(&b);

    float torque[exo_config::motors::AMOUNT] = {0};
    runner.update(angles, grounded, torque);

    TEST_ASSERT_EQUAL_INT(1, a.validityCalls);
    TEST_ASSERT_EQUAL_INT(1, b.validityCalls);
}

void test_zero_validity_context_still_selected()
{
    StubContext only(0.0f, 4.0f);
    ContextRunner runner;
    runner.registerContext(&only);

    float torque[exo_config::motors::AMOUNT] = {0};
    runner.update(angles, grounded, torque);

    TEST_ASSERT_EQUAL_INT(1, only.torqueCalls);
}

void test_no_context_gives_zero_torque()
{
    ContextRunner runner;
    float torque[exo_config::motors::AMOUNT];
    for (uint8_t i = 0; i < exo_config::motors::AMOUNT; ++i)
    {
        torque[i] = 5.0f;
    }

    runner.update(angles, grounded, torque);

    for (uint8_t i = 0; i < exo_config::motors::AMOUNT; ++i)
    {
        TEST_ASSERT_EQUAL_FLOAT(0.0f, torque[i]);
    }
}

void test_register_rejects_null_and_overflow()
{
    ContextRunner runner;
    TEST_ASSERT_FALSE(runner.registerContext(nullptr));

    StubContext stub(0.5f, 1.0f);
    for (uint8_t i = 0; i < exo_config::context::MAX_CONTEXTS; ++i)
    {
        TEST_ASSERT_TRUE(runner.registerContext(&stub));
    }
    TEST_ASSERT_FALSE(runner.registerContext(&stub));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_selects_highest_validity);
    RUN_TEST(test_all_contexts_compute_validity);   
    RUN_TEST(test_zero_validity_context_still_selected);
    RUN_TEST(test_no_context_gives_zero_torque);
    RUN_TEST(test_register_rejects_null_and_overflow);
    return UNITY_END();
}
