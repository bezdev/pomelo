#include "GLFWApp.h"

void glfwOnError(int error, const char* description)
{
    LOGE("glfwOnError: %d, %s", error, description);
#ifdef WIN32
    // print message in Windows popup dialog box
    MessageBox(NULL, description, "GLFW error", MB_OK);
#endif
}

GLFWApp::GLFWApp():
    m_ScreenWidth(800),
    m_ScreenHeight(450)
{
}

GLFWApp::~GLFWApp()
{

}

int GLFWApp::Initialize()
{
    glfwSetErrorCallback(glfwOnError);

    GLenum hr = glfwInit();
    LOGD("glfwInit: %d", hr);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "pomelo", NULL, NULL);
    if (m_Window == NULL)
    {
        LOGE("glfwCreateWindow failed.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, s_SetFramebufferSizeCallback);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOGE("Initialize GLAD failed.");
        return -1;
    }

    m_App.Initialize();

    glfwGetFramebufferSize(m_Window, &m_ScreenWidth, &m_ScreenHeight);
    m_App.UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);

     m_App.SetStartScene(SCENE_CUBE);

    return 0;
}

void GLFWApp::s_SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    GLFWApp::GetInstance()->SetFramebufferSizeCallback(window, width, height);
}

void GLFWApp::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) Exit();
}

void GLFWApp::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        TIME_FUNC(ProcessInput());
        TIME_FUNC(m_App.Run());
        TIME_FUNC(glfwSwapBuffers(m_Window));
        TIME_FUNC(glfwPollEvents());
    }
}

void GLFWApp::Exit()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void GLFWApp::SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    LOGD("App::SetFramebufferSizeCallback");
    m_App.UpdateWindowSize(width, height);
}