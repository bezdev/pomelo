#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>

#include <sys/stat.h>

#include "Test.h"

#include "app/GLFWApp.h"
#include "config.h"
#include "util/Logger.h"

#ifdef WIN32
#define PERF_RESULTS_OUTPUT_PATH "..\\..\\..\\PerformanceResults.csv"
#else
#define PERF_RESULTS_OUTPUT_PATH "PerformanceResults.csv"
#endif

void StartApp(SceneType sceneType)
{
    GLFWApp *app = GLFWApp::GetInstance();
    app->Initialize(sceneType);

    std::thread t = std::thread([app] {
        SLEEP(5000);
        app->Exit();
    });

    app->Run();

    t.join();

    GLFWApp::DestoryInstance();
}

void LogToFile(std::string testName, std::vector<std::string> log)
{
    int count = 0;
    float total = 0.f, min = 1000000.f, max = 0.f, avg = 0.f;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::string resolution;

    for (auto message : log)
    {
        auto split = Util::SplitString(message, std::string(" "));
        if (split.size() > 4 && split[3].compare(std::string("FPS:")) == 0)
        {
            count++;

            float current = std::stof(split[4]);
            total += current;
            if (current < min)
                min = current;
            if (current > max)
                max = current;
        }
        else if (split.size() > 4 && split[3].compare(std::string("App::UpdateWindowSize")) == 0)
        {
            resolution = std::string(split[4]);
        }
    }

    avg = total / count;

    struct stat buffer;
    auto fileExists = (stat(PERF_RESULTS_OUTPUT_PATH, &buffer) == 0);

    std::ofstream file;
    file.open(PERF_RESULTS_OUTPUT_PATH, std::ofstream::out | std::ofstream::app);
    if (!fileExists)
    {
        file << "time,OS,resolution,name,average,min,max" << std::endl;
    }

#ifdef WIN32
    std::string os("windows");
#else
    std::string os("macOS");
#endif

    file << std::put_time(&tm, "%Y-%m-%d %H-%M-%S") << "," << os << "," << resolution << "," << testName << "," << avg << "," << min << ","
         << max << std::endl;
    file.close();
}

TEST(SingleCubePerformanceTest)
{
    StartApp(SceneType::SCENE_CUBE);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST(ManyCubePerformanceTest)
{
    StartApp(SceneType::SCENE_MANY_CUBE);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST(ManyCubeInstancedPerformanceTest)
{
    StartApp(SceneType::SCENE_MANY_CUBE_INSTANCED);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST(ManyCubeAxisPerformanceTest)
{
    StartApp(SceneType::SCENE_MANY_CUBE_AXIS);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST(JoltScenePerformanceTest)
{
    StartApp(SceneType::SCENE_JOLT_HELLO_WORLD);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST_DISABLED(CollisionScenePerformanceTest)
{
    StartApp(SceneType::SCENE_COLLISION);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST_DISABLED(CollisionSceneNoPhysicsPerformanceTest)
{
    StartApp(SceneType::SCENE_COLLISION_NO_PHYSICS);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}

TEST_DISABLED(CollisionSceneNoCollisionsPerformanceTest)
{
    StartApp(SceneType::SCENE_COLLISION_NO_COLLISION);

    auto log = Logger::GetInstance()->GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
    Logger::GetInstance()->Clear();
}