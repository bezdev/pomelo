#pragma once

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <jni.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include <errno.h>
#include <vector>
#include <mutex>

#define NATIVE_ACTIVITY_CLASS_NAME "android/app/NativeActivity"
#endif
#ifdef BUILD_DESKTOP
//#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

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

#ifdef BUILD_DESKTOP
    static void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow* window);
#endif

    void Run();
    void UpdateWindowSize(int width, int height);
    void SetStartScene(int sceneId) { m_StartSceneId = sceneId; }
    void Exit() { m_ShouldExit = true; }
private:
    void LogFPS();

    Renderer* m_Renderer;
    Timer* m_GlobalTimer;

    bool m_ShouldExit;
    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_StartSceneId;

#ifdef BUILD_DESKTOP
    GLFWwindow* m_Window;
#endif
};
