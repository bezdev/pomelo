#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "app.h"
#include "util.h"
#include "timer.h"

#define NATIVE_ACTIVITY_CLASS_NAME "android/app/NativeActivity"

class GLFWApp
{
public:
    static GLFWApp* GetInstance()
    {
        if (!s_Instance) s_Instance = new GLFWApp();
        return s_Instance;
    };

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    };

    static void s_SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWApp();
    ~GLFWApp();
    // TODO: remove sceneId parameter
    int Initialize(int sceneId);

    void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();

    void Run();
    void Exit();
private:
    static GLFWApp* s_Instance;
    App m_App;

    GLFWwindow* m_Window;

    int m_ScreenWidth;
    int m_ScreenHeight;
};