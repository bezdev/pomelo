#include <thread>

#include "Test.h"

#include "timer.h"
#include "logger.h"

TEST(BasicTimerTest)
{
    BasicTimer t;
    t.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    float totalTime = t.GetTotalTime();
    LOGI("Elapsed time: %f", totalTime);
    ASSERT_ARE_EQUAL_DELTA(totalTime, 1.f, 0.01f)
}

TEST(ScopeTimerTest)
{
    ScopeTimer t;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}