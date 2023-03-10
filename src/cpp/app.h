#pragma once

#include "renderer.h"
#include "timer.h"
#include "util.h"

#define DEBUG 1

class Renderer;

class GameApp {
public:
    virtual void Initialize() = 0;
    virtual void Run() = 0;
};

class App {
public:
    App();
    ~App();

    static App* GetInstance()
    {
        static App* instance = new App();
        return instance;
    };

    int Initialize();

    void Run();
    void UpdateWindowSize(int width, int height);
    void SetStartScene(int sceneId) { m_StartSceneId = sceneId; }
private:
    void LogFPS();

    Renderer* m_Renderer;
    Timer* m_GlobalTimer;

    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_StartSceneId;
};
