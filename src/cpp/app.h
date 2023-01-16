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

class App {
public:
    App();
    ~App();

    static App* GetInstance()
    {
        static App* instance = new App();
        return instance;
    };

#ifdef BUILD_ANDROID
    bool IsReady();
    android_app* GetApp() { return m_App; }

    int Initialize(android_app*);
    void OnAppCommand(android_app*, int32_t);
#endif

#ifdef BUILD_DESKTOP
    int Initialize();
    static void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void processInput(GLFWwindow* window);
#endif

    void Run();
    void UpdateWindowSize(int width, int height);
    void SetStartScene(int sceneId) { m_StartSceneId = sceneId; }
    void Exit() { m_ShouldExit = true; }
private:
    void CalculateFrameStats();

    android_app* m_App;
    Renderer* m_Renderer;
    Timer* m_GlobalTimer;

    int m_ScreenWidth;
    int m_ScreenHeight;
    int m_StartSceneId;
    bool m_ShouldExit;

#ifdef BUILD_ANDROID
    bool m_HasFocus;
    bool m_IsVisible;
    bool m_HasWindow;
    static void OnAppCmd(struct android_app*, int32_t);
    static int32_t OnInputEvent(android_app*, AInputEvent*);
#endif
#ifdef BUILD_DESKTOP
    GLFWwindow* m_Window;
#endif
};
