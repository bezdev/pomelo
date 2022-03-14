#include "Test.h"

#include "util.h"
#include "timer.h"

TEST(StringSplitTestBasic)
{
    auto split = Util::StringSplit("a,b,c", ",");
    ASSERT_ARE_EQUAL(3, split.size());
    ASSERT_ARE_EQUAL("a", split[0]);
    ASSERT_ARE_EQUAL("b", split[1]);
    ASSERT_ARE_EQUAL("c", split[2]);
}

TEST(StringSplitTestEmptyDelimeter)
{
    auto split = Util::StringSplit("a,b,c", "");
    ASSERT_ARE_EQUAL(1, split.size());
    ASSERT_ARE_EQUAL("a,b,c", split[0]);
}

TEST(StringSplitTestEmptyNone)
{
    auto split = Util::StringSplit("a,b,c", "x");
    ASSERT_ARE_EQUAL(1, split.size());
    ASSERT_ARE_EQUAL("a,b,c", split[0]);
}

TEST(StringSplitTestLongDelim)
{
    auto split = Util::StringSplit("axxxxxbxxxxxc", "xxxxx");
    ASSERT_ARE_EQUAL(3, split.size());
    ASSERT_ARE_EQUAL("a", split[0]);
    ASSERT_ARE_EQUAL("b", split[1]);
    ASSERT_ARE_EQUAL("c", split[2]);
}

TEST(StringSplitPerfTest)
{
    std::string s = "a,b,c,d,e,f";
    std::string delimiter = ",";
    const int loopCount = 1e3;
    Timer t1;
    t1.Start();
    for (int i = 0; i < loopCount; i++)
    {
        auto result = Util::StringSplit(s, delimiter);

        ASSERT_ARE_EQUAL(6, result.size());
    }
    t1.Pause();

    Timer t2;
    t2.Start();
    for (int i = 0; i < loopCount; i++)
    {
        auto result = Util::StringSplit(s.c_str(), delimiter.c_str());
        ASSERT_ARE_EQUAL(6, result.size());
    }
    t2.Pause();

    printf("std approach: %f\n", t1.GetDelta());
    printf("bez approach: %f\n", t2.GetDelta());
    printf("%.2f%% gainz\n", 100.f * t1.GetDelta()/t2.GetDelta());
    ASSERT_TRUE(t1.GetDelta() > t2.GetDelta());
}
