#include <memory>
#include <thread>
#include <iostream>
#include <fstream>

#include "Test.h"

#include "app.h"

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
    std::ofstream file;
    file.open("PerformanceResults.csv", std::ofstream::out | std::ofstream::app);
    file << testName << std::endl;
    file.close();
}

void EndApp(App* app)
{
    app->Exit();
    RunAppThread.join();
}

DISABLED_TEST(SingleCubePerformanceTest)
{
    auto app = StartApp(SCENE_CUBE);

    SLEEP(5000);

    EndApp(app);

    auto log = TestSuite::GetInstance().GetLog();
    assert(log.size() > 0);

    LogToFile(GetName(), log);
}
