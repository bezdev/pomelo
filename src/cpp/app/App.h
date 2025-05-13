#pragma once

#include "Jolt/Jolt.h"

#include "engine/Camera.h"
#include "engine/InputManager.h"
#include "engine/Scene.h"
#include "render/Renderer.h"
#include "util/Timer.h"
#include "util/Util.h"

#define DEBUG 1

class Renderer;

using OnUpdateCallback = std::function<void(float)>;

class GameApp
{
  public:
    virtual void Initialize() = 0;
    virtual void Run() = 0;
};

class App
{
  public:
    static App *GetInstance()
    {
        if (!s_Instance)
            s_Instance = new App();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    App();
    ~App();

    int Initialize();

    void Run();
    void OnInputEvent(InputEvent event, InputData data)
    {
        InputManager::GetInstance()->OnEvent(event, data);
    }
    void UpdateWindowSize(int width, int height);
    void SetStartScene(SceneType sceneType)
    {
        m_StartSceneType = sceneType;
    }

  private:
    static App *s_Instance;

    void LogFPS();

    Renderer *m_Renderer;
    Timer *m_GlobalTimer;

    bool m_IsFirstFrame;
    int m_ScreenWidth;
    int m_ScreenHeight;
    SceneType m_StartSceneType;

    std::vector<OnUpdateCallback> m_Callbacks;
};
