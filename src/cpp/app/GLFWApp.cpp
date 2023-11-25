#include "GLFWApp.h"

GLFWApp* GLFWApp::s_Instance = nullptr;

#ifdef WIN32
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#else
#define SCREEN_WIDTH 1600 
#define SCREEN_HEIGHT 900
#endif

void glfwOnError(int error, const char* description)
{
    LOGE("glfwOnError: %d, %s", error, description);
#ifdef WIN32
    // print message in Windows popup dialog box
    MessageBox(NULL, description, "GLFW error", MB_OK);
#endif
}

GLFWApp::GLFWApp():
    m_ScreenWidth(SCREEN_WIDTH),
    m_ScreenHeight(SCREEN_HEIGHT)
{
}

GLFWApp::~GLFWApp()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
    LOGI("GLFWApp::~GLFWApp");
}

int GLFWApp::Initialize(int sceneId)
{
    glfwSetErrorCallback(glfwOnError);

    GLenum hr = glfwInit();
    LOGD("glfwInit: %d", hr);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "pomelo", NULL, NULL);
    if (m_Window == NULL)
    {
        LOGE("glfwCreateWindow failed.");
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(m_Window, s_KeyCallback);
    glfwSetMouseButtonCallback(m_Window, s_MouseButtonCallback);
    glfwSetCursorPosCallback(m_Window, s_CursorPositionCallback);

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

    m_App.SetStartScene(sceneId);

    return 0;
}

void GLFWApp::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) Exit();
}

void GLFWApp::Run()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        TIME_FUNC(glfwPollEvents());
        TIME_FUNC(m_App.Run());
        TIME_FUNC(glfwSwapBuffers(m_Window));
    }
}

void GLFWApp::Exit()
{
    glfwSetWindowShouldClose(m_Window, true);
}

void GLFWApp::s_SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    GLFWApp::GetInstance()->SetFramebufferSizeCallback(window, width, height);
}

void GLFWApp::s_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    GLFWApp::GetInstance()->KeyCallback(window, key, scancode, action, mods);
}

void GLFWApp::s_MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    GLFWApp::GetInstance()->MouseButtonCallback(window, button, action, mods);
}

void GLFWApp::s_CursorPositionCallback(GLFWwindow *window, double xpos, double ypos)
{
    GLFWApp::GetInstance()->CursorPositionCallback(window, xpos, ypos);
}

void GLFWApp::SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    LOGD("App::SetFramebufferSizeCallback");
    m_App.UpdateWindowSize(width, height);
}

void GLFWApp::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (GLFWKeyToInputEventMap.count(key) != 0)
    {
        InputAction a = InputAction::DOWN;
        if (action == GLFW_RELEASE) a = InputAction::UP;
        m_App.OnInputEvent(GLFWKeyToInputEventMap[key], InputData(a));
    }
}

void GLFWApp::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (GLFWMouseToInputEventMap.count(button) != 0)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        InputAction a = InputAction::DOWN;
        if (action == GLFW_RELEASE) a = InputAction::UP;
        m_App.OnInputEvent(
            GLFWMouseToInputEventMap[button],
            InputData(a, static_cast<float>(x), static_cast<float>(y))
        );
    }
}

void GLFWApp::CursorPositionCallback(GLFWwindow* window, double x, double y)
{
    InputManager::GetInstance()->OnEvent(InputEvent::MOUSE_MOVE, InputData(InputAction::MOVE, static_cast<float>(x), static_cast<float>(y)));
}

std::map<int, InputEvent> GLFWApp::GLFWKeyToInputEventMap = {
    { GLFW_KEY_SPACE, InputEvent::KEY_SPACE },
    { GLFW_KEY_APOSTROPHE, InputEvent::KEY_APOSTROPHE },
    { GLFW_KEY_COMMA, InputEvent::KEY_COMMA },
    { GLFW_KEY_MINUS, InputEvent::KEY_MINUS },
    { GLFW_KEY_PERIOD, InputEvent::KEY_PERIOD },
    { GLFW_KEY_SLASH, InputEvent::KEY_SLASH },
    { GLFW_KEY_0, InputEvent::KEY_0 },
    { GLFW_KEY_1, InputEvent::KEY_1 },
    { GLFW_KEY_2, InputEvent::KEY_2 },
    { GLFW_KEY_3, InputEvent::KEY_3 },
    { GLFW_KEY_4, InputEvent::KEY_4 },
    { GLFW_KEY_5, InputEvent::KEY_5 },
    { GLFW_KEY_6, InputEvent::KEY_6 },
    { GLFW_KEY_7, InputEvent::KEY_7 },
    { GLFW_KEY_8, InputEvent::KEY_8 },
    { GLFW_KEY_9, InputEvent::KEY_9 },
    { GLFW_KEY_SEMICOLON, InputEvent::KEY_SEMICOLON },
    { GLFW_KEY_EQUAL, InputEvent::KEY_EQUAL },
    { GLFW_KEY_A, InputEvent::KEY_A },
    { GLFW_KEY_B, InputEvent::KEY_B },
    { GLFW_KEY_C, InputEvent::KEY_C },
    { GLFW_KEY_D, InputEvent::KEY_D },
    { GLFW_KEY_E, InputEvent::KEY_E },
    { GLFW_KEY_F, InputEvent::KEY_F },
    { GLFW_KEY_G, InputEvent::KEY_G },
    { GLFW_KEY_H, InputEvent::KEY_H },
    { GLFW_KEY_I, InputEvent::KEY_I },
    { GLFW_KEY_J, InputEvent::KEY_J },
    { GLFW_KEY_K, InputEvent::KEY_K },
    { GLFW_KEY_L, InputEvent::KEY_L },
    { GLFW_KEY_M, InputEvent::KEY_M },
    { GLFW_KEY_N, InputEvent::KEY_N },
    { GLFW_KEY_O, InputEvent::KEY_O },
    { GLFW_KEY_P, InputEvent::KEY_P },
    { GLFW_KEY_Q, InputEvent::KEY_Q },
    { GLFW_KEY_R, InputEvent::KEY_R },
    { GLFW_KEY_S, InputEvent::KEY_S },
    { GLFW_KEY_T, InputEvent::KEY_T },
    { GLFW_KEY_U, InputEvent::KEY_U },
    { GLFW_KEY_V, InputEvent::KEY_V },
    { GLFW_KEY_W, InputEvent::KEY_W },
    { GLFW_KEY_X, InputEvent::KEY_X },
    { GLFW_KEY_Y, InputEvent::KEY_Y },
    { GLFW_KEY_Z, InputEvent::KEY_Z },
    { GLFW_KEY_LEFT_BRACKET, InputEvent::KEY_LEFT_BRACKET },
    { GLFW_KEY_BACKSLASH, InputEvent::KEY_BACKSLASH },
    { GLFW_KEY_RIGHT_BRACKET, InputEvent::KEY_RIGHT_BRACKET },
    { GLFW_KEY_GRAVE_ACCENT, InputEvent::KEY_GRAVE_ACCENT },
    { GLFW_KEY_ESCAPE, InputEvent::KEY_ESCAPE },
    { GLFW_KEY_ENTER, InputEvent::KEY_ENTER },
    { GLFW_KEY_TAB, InputEvent::KEY_TAB },
    { GLFW_KEY_BACKSPACE, InputEvent::KEY_BACKSPACE },
    { GLFW_KEY_INSERT, InputEvent::KEY_INSERT },
    { GLFW_KEY_DELETE, InputEvent::KEY_DELETE },
    { GLFW_KEY_RIGHT, InputEvent::KEY_RIGHT },
    { GLFW_KEY_LEFT, InputEvent::KEY_LEFT },
    { GLFW_KEY_DOWN, InputEvent::KEY_DOWN },
    { GLFW_KEY_UP, InputEvent::KEY_UP },
    { GLFW_KEY_PAGE_UP, InputEvent::KEY_PAGE_UP },
    { GLFW_KEY_PAGE_DOWN, InputEvent::KEY_PAGE_DOWN },
    { GLFW_KEY_HOME, InputEvent::KEY_HOME },
    { GLFW_KEY_END, InputEvent::KEY_END },
    { GLFW_KEY_CAPS_LOCK, InputEvent::KEY_CAPS_LOCK },
    { GLFW_KEY_SCROLL_LOCK, InputEvent::KEY_SCROLL_LOCK },
    { GLFW_KEY_NUM_LOCK, InputEvent::KEY_NUM_LOCK },
    { GLFW_KEY_PRINT_SCREEN, InputEvent::KEY_PRINT_SCREEN },
    { GLFW_KEY_PAUSE, InputEvent::KEY_PAUSE },
    { GLFW_KEY_F1, InputEvent::KEY_F1 },
    { GLFW_KEY_F2, InputEvent::KEY_F2 },
    { GLFW_KEY_F3, InputEvent::KEY_F3 },
    { GLFW_KEY_F4, InputEvent::KEY_F4 },
    { GLFW_KEY_F5, InputEvent::KEY_F5 },
    { GLFW_KEY_F6, InputEvent::KEY_F6 },
    { GLFW_KEY_F7, InputEvent::KEY_F7 },
    { GLFW_KEY_F8, InputEvent::KEY_F8 },
    { GLFW_KEY_F9, InputEvent::KEY_F9 },
    { GLFW_KEY_F10, InputEvent::KEY_F10 },
    { GLFW_KEY_F11, InputEvent::KEY_F11 },
    { GLFW_KEY_F12, InputEvent::KEY_F12 },
    { GLFW_KEY_F13, InputEvent::KEY_F13 },
    { GLFW_KEY_F14, InputEvent::KEY_F14 },
    { GLFW_KEY_F15, InputEvent::KEY_F15 },
    { GLFW_KEY_F16, InputEvent::KEY_F16 },
    { GLFW_KEY_F17, InputEvent::KEY_F17 },
    { GLFW_KEY_F18, InputEvent::KEY_F18 },
    { GLFW_KEY_F19, InputEvent::KEY_F19 },
    { GLFW_KEY_F20, InputEvent::KEY_F20 },
    { GLFW_KEY_F21, InputEvent::KEY_F21 },
    { GLFW_KEY_F22, InputEvent::KEY_F22 },
    { GLFW_KEY_F23, InputEvent::KEY_F23 },
    { GLFW_KEY_F24, InputEvent::KEY_F24 },
    { GLFW_KEY_F25, InputEvent::KEY_F25 },
    { GLFW_KEY_KP_0, InputEvent::KEY_KP_0 },
    { GLFW_KEY_KP_1, InputEvent::KEY_KP_1 },
    { GLFW_KEY_KP_2, InputEvent::KEY_KP_2 },
    { GLFW_KEY_KP_3, InputEvent::KEY_KP_3 },
    { GLFW_KEY_KP_4, InputEvent::KEY_KP_4 },
    { GLFW_KEY_KP_5, InputEvent::KEY_KP_5 },
    { GLFW_KEY_KP_6, InputEvent::KEY_KP_6 },
    { GLFW_KEY_KP_7, InputEvent::KEY_KP_7 },
    { GLFW_KEY_KP_8, InputEvent::KEY_KP_8 },
    { GLFW_KEY_KP_9, InputEvent::KEY_KP_9 },
    { GLFW_KEY_KP_DECIMAL, InputEvent::KEY_KP_DECIMAL },
    { GLFW_KEY_KP_DIVIDE, InputEvent::KEY_KP_DIVIDE },
    { GLFW_KEY_KP_MULTIPLY, InputEvent::KEY_KP_MULTIPLY },
    { GLFW_KEY_KP_SUBTRACT, InputEvent::KEY_KP_SUBTRACT },
    { GLFW_KEY_KP_ADD, InputEvent::KEY_KP_ADD },
    { GLFW_KEY_KP_ENTER, InputEvent::KEY_KP_ENTER },
    { GLFW_KEY_KP_EQUAL, InputEvent::KEY_KP_EQUAL },
    { GLFW_KEY_LEFT_SHIFT, InputEvent::KEY_LEFT_SHIFT },
    { GLFW_KEY_LEFT_CONTROL, InputEvent::KEY_LEFT_CONTROL },
    { GLFW_KEY_LEFT_ALT, InputEvent::KEY_LEFT_ALT },
    { GLFW_KEY_LEFT_SUPER, InputEvent::KEY_LEFT_SUPER },
    { GLFW_KEY_RIGHT_SHIFT, InputEvent::KEY_RIGHT_SHIFT },
    { GLFW_KEY_RIGHT_CONTROL, InputEvent::KEY_RIGHT_CONTROL },
    { GLFW_KEY_RIGHT_ALT, InputEvent::KEY_RIGHT_ALT },
    { GLFW_KEY_RIGHT_SUPER, InputEvent::KEY_RIGHT_SUPER },
};

std::map<int, InputEvent> GLFWApp::GLFWMouseToInputEventMap = {
    { GLFW_MOUSE_BUTTON_LEFT, InputEvent::MOUSE_BUTTON_LEFT },
    { GLFW_MOUSE_BUTTON_RIGHT, InputEvent::MOUSE_BUTTON_RIGHT },
    { GLFW_MOUSE_BUTTON_MIDDLE, InputEvent::MOUSE_BUTTON_MIDDLE },
    { GLFW_MOUSE_BUTTON_4, InputEvent::MOUSE_BUTTON_4 },
    { GLFW_MOUSE_BUTTON_5, InputEvent::MOUSE_BUTTON_5 },
};