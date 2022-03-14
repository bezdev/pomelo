#include <iostream>

#include "Test.h"

TestSuite::TestSuite() { }

void TestSuite::Run()
{
    std::vector<std::string> failedTests;
    std::string currentTestName;
    bool isFail = false;

    std::cout << "Running tests..." << std::endl;

    for (int i = 0; i < m_Tests.size(); i++)
    {
        try
        {
            auto test = m_Tests[i]();
            currentTestName = test->GetName();
            test->Run();
            isFail = false;
        }
        catch (int)
        {
            failedTests.push_back(currentTestName);
            isFail = true;
        }

        if (!isFail)
        {
            printf("SUCCEEDED [%s]\n", currentTestName.c_str());
        }
    }

    int totalTests = m_Tests.size();
    int succeededTestCount = m_Tests.size() - failedTests.size();
    int failedTestCount = failedTests.size();
    printf("\n  COMPLETED %d TESTS: %d/%d (%.2f%%)\n",
        totalTests,
        succeededTestCount,
        totalTests,
        100.f * succeededTestCount / totalTests);

    if (failedTestCount > 0)
    {
        std::cout << std::endl << "  Failed tests:" << std::endl;

        for (auto testName : failedTests)
        {
            std::cout << "    " << testName << std::endl;
        }
    }
}

void TestSuite::AddTest(std::function<ITest*()> function)
{
    m_Tests.push_back(function);
}
