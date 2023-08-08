#pragma once

#include "engine/InputManager.h"
#include "engine/Camera.h"
#include "render/Renderer.h"
#include "util/Timer.h"
#include "util/Util.h"

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

    int Initialize();

    void Run();
    void OnInputEvent(InputEvent event, InputData data) { InputManager::GetInstance()->OnEvent(event, data); }
    void UpdateWindowSize(int width, int height);
    void SetStartScene(int sceneId) { m_StartSceneId = sceneId; }
private:
    void LogFPS();

    Renderer* m_Renderer;
    Timer* m_GlobalTimer;

    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_StartSceneId;

    bool m_IsFirstFrame;
};
