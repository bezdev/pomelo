#include "Test.h"

#include "util/Util.h"
#include "util/Timer.h"

std::vector<std::string> StringSplit(const char* string, const char* delimiter)
{
    std::vector<std::string> result;

    const int bufferSize = 1000;
    char currentSplitBuffer[bufferSize];
    int currentSplitLength = 0;
    bool isInDelimiter = false;
    int delimiterIndex = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (i >= bufferSize) throw;

        char currentChar = string[i];
        currentSplitBuffer[currentSplitLength++] = currentChar;


        if (!isInDelimiter)
        {
            if (currentChar == delimiter[0])
            {
                delimiterIndex = 0;
                isInDelimiter = true;
            }
        }

        if (isInDelimiter)
        {
            if (currentChar != delimiter[delimiterIndex])
            {
                isInDelimiter = false;

                continue;
            }

            if (delimiter[delimiterIndex + 1] == '\0')
            {
                isInDelimiter = false;

                int actualSplitLength = currentSplitLength - (delimiterIndex + 1);
                if (actualSplitLength > 0)
                {
                    result.push_back(std::string(currentSplitBuffer, actualSplitLength));
                }
                currentSplitLength = 0;

                continue;
            }

            delimiterIndex++;
        }

        if (string[i + 1] == '\0')
        {
            int actualSplitLength = currentSplitLength;
            if (actualSplitLength > 0)
            {
                result.push_back(std::string(currentSplitBuffer, actualSplitLength));
            }
        }
    }

    return result;
}

TEST(StringSplitTestBasic)
{
    auto split = StringSplit("a,b,c", ",");
    ASSERT_ARE_EQUAL(3, split.size());
    ASSERT_ARE_EQUAL("a", split[0]);
    ASSERT_ARE_EQUAL("b", split[1]);
    ASSERT_ARE_EQUAL("c", split[2]);
}

TEST(StringSplitTestEmptyDelimeter)
{
    auto split = StringSplit("a,b,c", "");
    ASSERT_ARE_EQUAL(1, split.size());
    ASSERT_ARE_EQUAL("a,b,c", split[0]);
}

TEST(StringSplitTestEmptyNone)
{
    auto split = StringSplit("a,b,c", "x");
    ASSERT_ARE_EQUAL(1, split.size());
    ASSERT_ARE_EQUAL("a,b,c", split[0]);
}

TEST(StringSplitTestLongDelim)
{
    auto split = StringSplit("axxxxxbxxxxxc", "xxxxx");
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

    {
        ScopeTimer t("std implementation");
        for (int i = 0; i < loopCount; i++)
        {
            auto result = Util::SplitString(s, delimiter);

            ASSERT_ARE_EQUAL(6, result.size());
        }
    }

    {
        ScopeTimer t("bez implementation");
        for (int i = 0; i < loopCount; i++)
        {
            auto result = StringSplit(s.c_str(), delimiter.c_str());
            ASSERT_ARE_EQUAL(6, result.size());
        }
    }
}
