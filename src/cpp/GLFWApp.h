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
    static void s_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void s_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void s_CursorPositionCallback(GLFWwindow* window, double x, double y);
    void SetFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void CursorPositionCallback(GLFWwindow* window, double x, double y);

    GLFWApp();
    ~GLFWApp();

    // TODO: remove sceneId parameter
    int Initialize(int sceneId);

    void ProcessInput();

    void Run();
    void Exit();
private:
    static GLFWApp* s_Instance;
    App m_App;

    GLFWwindow* m_Window;

    int m_ScreenWidth;
    int m_ScreenHeight;

    static std::map<int, InputEvent> GLFWKeyToInputEventMap;
    static std::map<int, InputEvent> GLFWMouseToInputEventMap;
};