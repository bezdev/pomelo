#include <thread>

#include "Test.h"

#include "util/Timer.h"
#include "util/Logger.h"

#ifdef _WIN32
#define DELTA 0.02f
#else
#define DELTA 0.01f
#endif

TEST(BasicTimerTest)
{
    BasicTimer t;
    t.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    float totalTime = t.GetTotalTime();
    LOGT("total time: %f", totalTime);
    ASSERT_ARE_EQUAL_DELTA(totalTime, 1.f, DELTA)
}

TEST(ScopeTimerTest)
{
    ScopeTimer t("ScopeTimerTest");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}