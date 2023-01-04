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

#define PERF_RESULTS_OUTPUT_PATH PERF_RESULTS_PATH "PerformanceResults.csv"

std::thread RunAppThread;

App* StartApp(int sceneId)
{
    App* app = App::GetInstance();
    
    RunAppThread = std::thread([app, sceneId]
    { 
        app->SetStartScene(sceneId);
        app->Initialize();
        app->Run(); 
    });

    return app;
}

void LogToFile(std::string testName, std::vector<std::string> log)
{
    int count = 0;
    float total = 0.f, min = 1000000.f, max = 0.f, avg = 0.f;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    for (auto message : log)
    {
        auto split = Util::StringSplit(message, std::string(" "));
        if (split[0].compare(std::string("FPS:")) == 0)
        {
            count++;

            float current = std::stof(split[1]);
            total += current;
            if (current < min) min = current;
            if (current > max) max = current;
        }
    }

    avg = total / count;

    struct stat buffer;
    auto fileExists = (stat(PERF_RESULTS_OUTPUT_PATH, &buffer) == 0);

    std::ofstream file;
    file.open(PERF_RESULTS_OUTPUT_PATH, std::ofstream::out | std::ofstream::app);
    if (!fileExists)
    {
        file << "time,name,average,min,max" << std::endl;
    }

    file << std::put_time(&tm, "%Y-%m-%d %H-%M-%S") << "," << testName << "," << avg << "," << min << "," << max << std::endl;    
    file.close();
}

void EndApp(App* app)
{
    app->Exit();
    RunAppThread.join();
}

TEST(SingleCubePerformanceTest)
{
    auto app = StartApp(SCENE_CUBE);

    SLEEP(5000);

    EndApp(app);

    auto log = TestSuite::GetInstance().GetLog();
    assert(log.size() > 0);

    LogToFile(GetName(), log);
}
