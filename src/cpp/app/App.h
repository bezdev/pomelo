#pragma once

#include "engine/InputManager.h"
#include "engine/Camera.h"
#include "render/Renderer.h"
#include "engine/Scene.h"
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
    void SetStartScene(SceneType sceneType) { m_StartSceneType = sceneType; }
private:
    void LogFPS();

    Renderer* m_Renderer;
    Timer* m_GlobalTimer;

    int m_ScreenWidth;
    int m_ScreenHeight;
    SceneType m_StartSceneType;

    bool m_IsFirstFrame;
};
