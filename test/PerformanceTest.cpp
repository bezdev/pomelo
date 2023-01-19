#include <memory>
#include <thread>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>

#include <sys/stat.h>

#include "Test.h"

#include "app.h"
#include "config.h"
#include "logger.h"

#define PERF_RESULTS_OUTPUT_PATH PERF_RESULTS_PATH "PerformanceResults.csv"

// TODO: must do this on a main thread
App* StartApp(int sceneId)
{
    App* app = App::GetInstance();
    app->SetStartScene(sceneId);
    app->Initialize();

    std::thread t = std::thread([app, sceneId]
    { 
        SLEEP(5000);
        app->Exit();
    });

    app->Run(); 

    t.join();

    return app;
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
        auto split = Util::StringSplit(message, std::string(" "));
        LOGT("size: %d", split.size());
        if (split.size() > 1 && split[1].compare(std::string("FPS:")) == 0)
        {
            count++;

            float current = std::stof(split[2]);
            total += current;
            if (current < min) min = current;
            if (current > max) max = current;
        }
        else if (split.size() > 2 && split[1].compare(std::string("App::UpdateWindowSize")) == 0)
        {
            resolution = std::string(split[2]);
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

    int width = 1600;
    int height = 900;
    file << std::put_time(&tm, "%Y-%m-%d %H-%M-%S") << "," << os << "," << resolution << "," << testName << "," << avg << "," << min << "," << max << std::endl;    
    file.close();
}

TEST(SingleCubePerformanceTest)
{
    StartApp(SCENE_CUBE);

    // LOGT("app ended");
    auto log = Logger::GetInstance()->GetLog(); //TestSuite::GetInstance().GetLog();
    ASSERT_TRUE(log.size() > 0);

    LogToFile(GetName(), log);
}
