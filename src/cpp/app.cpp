#include "app.h"

#if 0
#include "jniutil.h"
#endif
#ifdef BUILD_DESKTOP
#endif

App::App()
#ifdef BUILD_DESKTOP
    :
    m_ScreenWidth(1600),
    m_ScreenHeight(900)
#endif
{ }

App::~App()
{
    LOGD("App::~App");

#ifdef BUILD_DESKTOP
    glfwTerminate();
#endif

    delete Renderer::GetInstance();

#if 0
    delete JNIUtil::GetInstance();
#endif

    delete m_GlobalTimer;
}

#ifdef BUILD_DESKTOP
void glfwOnError(int error, const char* description)
{
#ifdef WIN32
    // print message in Windows popup dialog box
    MessageBox(NULL, description, "GLFW error", MB_OK);
#endif
}
#endif

int App::Initialize()
{
#ifdef BUILD_DESKTOP
    glfwSetErrorCallback(glfwOnError);

    /*GLenum err = */glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "pomelo", NULL, NULL);
    if (m_Window == NULL)
    {
        // std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, SetFramebufferSizeCallback);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOGE("Failed to initialize GLAD");
        return -1;
    }
#endif

    m_GlobalTimer = new Timer();
    m_Renderer = Renderer::GetInstance();
    m_Renderer->Initialize();

#if 0
    JNIUtil::GetInstance()->Initialize(this);
#endif

    return 0;
}

void App::Run() {
    static bool isFirstFrame = true;

#ifdef BUILD_DESKTOP
    while (!glfwWindowShouldClose(m_Window))
#endif
    {
#ifdef BUILD_DESKTOP
        processInput(m_Window);
#endif
        if (isFirstFrame) {
            LOGD("first frame");

#ifdef BUILD_DESKTOP
            m_Renderer->Initialize();

            glfwGetFramebufferSize(m_Window, &m_ScreenWidth, &m_ScreenHeight);
            UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);

#endif
            // TODO: creation of meshes should happen dynamically
            Mesh::CreateBoxMesh(1.f, 1.f, 1.f);

            SceneManager::LoadScene(m_StartSceneId);

            m_GlobalTimer->Reset();
            isFirstFrame = false;
        } else {
            m_GlobalTimer->Update();
        }

        m_Renderer->Render();

        LogFPS();
#ifdef BUILD_DESKTOP
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
#endif
    }
}

#ifdef BUILD_DESKTOP
void App::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void App::SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    LOGD("App::SetFramebufferSizeCallback");
    App* app = App::GetInstance();
    app->UpdateWindowSize(width, height);
}
#endif

void App::UpdateWindowSize(int width, int height)
{
    LOGD("App::UpdateWindowSize %dx%d ", width, height);
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    m_Renderer->UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);
}

void App::LogFPS() {
    static int frameCount = 0;
    static float timeElapsed = 0.0f;

    frameCount++;
    timeElapsed += m_GlobalTimer->GetDelta();

    if (timeElapsed >= 1000.0f)
    {
        float fps = frameCount * 1000.f / timeElapsed;

        // Log
        LOGI("FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTotalTime(), frameCount);
#if 0
        static char buffer[100];
        sprintf(buffer, "FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTotalTime(), frameCount);
        JNIUtil::GetInstance()->LogTrigger(buffer);
#endif

        frameCount = 0;
        timeElapsed -= 1000.0f;
    }
}
