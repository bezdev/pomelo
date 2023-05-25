#pragma once

#include <functional>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <cmath>
#include <cassert>

#define ASSERT_TRUE(condition) do { if (!(condition)) { printf("FAILED [%s] %s:%d\n  %s\n", GetName().c_str(), __FILE__, __LINE__, #condition); throw 1; } } while(0);
#define ASSERT_ARE_EQUAL(a, b) do { ASSERT_TRUE(a == b) } while(0);
#define ASSERT_ARE_EQUAL_DELTA(a, b, delta) do { ASSERT_TRUE(std::abs(a - b) < delta) } while(0);
#define ASSERT_THROWS(expression) try { expression; assert(false); } catch (...) { assert(true); }

#define SLEEP(ms) do { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); } while(0);

class ITest
{
public:
    virtual void Run() = 0;
    virtual std::string GetName() = 0;
};

class TestSuite
{
public:
    TestSuite();
    static TestSuite& GetInstance()
    {
        static TestSuite t;
        return t;
    }

    void Run();
    void AddTest(std::function<ITest*()> function);
private:
    std::vector<std::function<ITest*()>> m_Tests;
};

#define TEST_BASE(name, isDisabled)                                                                      \
class Test##_##name : public ITest {                                                    \
public:                                                                                 \
    static ITest* GetInstance() { static Test##_##name t; return &t; }                  \
    Test##_##name() { if (!isDisabled) TestSuite::GetInstance().AddTest(Test##_##name::GetInstance); }   \
    void Run();                                                                         \
    std::string GetName() { return std::string(#name); }                                \
};                                                                                      \
ITest* name = Test##_##name::GetInstance();                                             \
void Test##_##name::Run()

#define TEST(name) TEST_BASE(name, false)
#define DISABLED_TEST(name) TEST_BASE(name, true)
