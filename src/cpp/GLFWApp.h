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
        static GLFWApp* instance = new GLFWApp();
        return instance;
    };

    static void s_SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);

    GLFWApp();
    ~GLFWApp();
    int Initialize();

    void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput();

    void Run();
    void Exit();
private:
    App m_App;

    GLFWwindow* m_Window;

    int m_ScreenWidth;
    int m_ScreenHeight;
};