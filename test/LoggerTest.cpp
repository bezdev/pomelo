#include "Test.h"

#include "util/Logger.h"

TEST(BasicLoggerTest)
{
    Logger::GetInstance()->Log(Logger::Level::Debug, "%d + %d = %d", 2, 3, 5);
    LOGD("debug: %f", 69.f);
    LOGI("info: %f", 420.f);
    LOGE("error: %s", "007");
    ASSERT_TRUE(true);
}