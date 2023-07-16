#include "app.h"

#include "PhysicsEngine.h"

App::App():
    m_IsFirstFrame(true)
{
}

App::~App()
{
    LOGD("App::~App");
    PhysicsEngine::DestroyInstance();
    Renderer::DestroyInstance();
    Camera::DestroyInstance();
    ECS::DestoryInstance();

#if 0
    delete JNIUtil::GetInstance();
#endif

    delete m_GlobalTimer;
}

int App::Initialize()
{
    m_GlobalTimer = new Timer();
    m_Renderer = Renderer::GetInstance();
    m_Renderer->Initialize();

#if 0
    JNIUtil::GetInstance()->Initialize(this);
#endif

    return 0;
}

void App::Run() {
    if (m_IsFirstFrame) {
        LOGD("first frame");

        SceneManager::LoadScene(m_StartSceneId);

        m_GlobalTimer->Reset();
        m_IsFirstFrame = false;
    } else {
        m_GlobalTimer->Update();
    }

    PhysicsEngine::GetInstance()->UpdateMotions(m_GlobalTimer->GetDelta());

    m_Renderer->Render();

    LogFPS();
}


void App::UpdateWindowSize(int width, int height)
{
    LOGD("App::UpdateWindowSize %dx%d ", width, height);
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    m_Renderer->UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);
}

void App::LogFPS()
{
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
