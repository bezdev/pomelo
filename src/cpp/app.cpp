#include "app.h"

#ifdef BUILD_ANDROID
#include "jniutil.h"
#endif
#ifdef BUILD_DESKTOP
#endif

App::App() :
    m_ShouldExit(false),
#ifdef BUILD_ANDROID
    m_HasFocus(false),
    m_HasWindow(false),
    m_IsVisible(false)
#endif
#ifdef BUILD_DESKTOP
    m_ScreenWidth(800),
    m_ScreenHeight(600)
#endif
{ }

App::~App()
{
    LOGD("App::~App");

#ifdef BUILD_DESKTOP
    glfwTerminate();
#endif

    delete Renderer::GetInstance();

#ifdef BUILD_ANDROID
    delete JNIUtil::GetInstance();
#endif

    delete m_GlobalTimer;
}

#ifdef BUILD_ANDROID
bool App::IsReady()
{
    return m_HasFocus && m_HasWindow && m_IsVisible;
}
#endif

#ifdef BUILD_DESKTOP
void glfwOnError(int error, const char* description)
{
    // print message in Windows popup dialog box
    MessageBox(NULL, description, "GLFW error", MB_OK);
}
#endif

#ifdef BUILD_ANDROID
int App::Initialize(android_app* androidApp)
#endif
#ifdef BUILD_DESKTOP
int App::Initialize()
#endif
{
#ifdef BUILD_ANDROID
    m_App = androidApp;
    m_App->userData = this;
    m_App->onAppCmd = App::OnAppCmd;
    m_App->onInputEvent = App::OnInputEvent;
#endif
#ifdef BUILD_DESKTOP
    glfwSetErrorCallback(glfwOnError);

    GLenum err = glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "fame", NULL, NULL);
    if (m_Window == NULL)
    {
        // std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_Window);
    glfwSetFramebufferSizeCallback(m_Window, SetFramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOGE("Failed to initialize GLAD");
        return -1;
    }
#endif

    m_GlobalTimer = new Timer();
    m_Renderer = Renderer::GetInstance();

#ifdef BUILD_ANDROID
    JNIUtil::GetInstance()->Initialize(this);
#endif
#ifdef BUILD_DESKTOP
    UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);
#endif

    return 0;
}

void App::Run() {
    bool isReady;
    bool isFirstFrame = true;

#ifdef BUILD_ANDROID
    while (true)
#endif
#ifdef BUILD_DESKTOP
    while (!glfwWindowShouldClose(m_Window))
#endif
    {
#ifdef BUILD_ANDROID
        int id;
        int events;
        android_poll_source *source;
        bool shouldDestroy = false;
        while ((id = ALooper_pollAll((isReady = IsReady()) ? 0 : -1, NULL, &events,
                                     (void **) &source)) >= 0) {
            if (shouldDestroy) continue;

            LOGD("BEZDEBUG m_App %p", &m_App);
            if (source != NULL) source->process(m_App, source);

            if (m_App->destroyRequested != 0) {
                shouldDestroy = true;
                return;
            }
        }

        if (shouldDestroy) return;

        if (!isReady) continue;
#endif
#ifdef BUILD_DESKTOP
        processInput(m_Window);
#endif
        if (m_ShouldExit) return;

        if (isFirstFrame) {
            LOGD("first frame");
            m_Renderer->Initialize();

            // TODO: creation of meshes should happen dynamically
            Mesh::CreateBoxMesh(1.f, 1.f, 1.f);

            SceneManager::LoadScene(m_StartSceneId);

            m_GlobalTimer->Reset();
            isFirstFrame = false;
        } else {
            m_GlobalTimer->Update();
        }

        m_Renderer->Render();

        CalculateFrameStats();

#ifdef BUILD_DESKTOP
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
#endif
    }
}

#ifdef BUILD_ANDROID
void App::OnAppCmd(struct android_app* androidApp, int32_t command) {
    App* app = (App*)androidApp->userData;
    app->OnAppCommand(androidApp, command);
}

void App::OnAppCommand(android_app* androidApp, int32_t command) {
    switch (command) {
        case APP_CMD_SAVE_STATE:
            LOGI("APP_CMD_SAVE_STATE");
            break;
        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW");
            //m_HasFocus = true; // TODO: maybe add abck
            m_HasWindow = true;
            //Renderer::GetInstance()->Initialize(androidApp);
            //app->Initialize(androidApp);
            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");
            m_HasWindow = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            LOGI("APP_CMD_GAINED_FOCUS");
            m_HasFocus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            LOGI("APP_CMD_LOST_FOCUS");
            m_HasFocus = false;
            break;
        case APP_CMD_LOW_MEMORY:
            LOGI("APP_CMD_LOW_MEMORY");
            break;
        case APP_CMD_STOP:
            LOGI("APP_CMD_STOP");
            m_IsVisible = false;
            break;
        case APP_CMD_PAUSE:
            LOGI("APP_CMD_PAUSE");
            break;
        case APP_CMD_RESUME:
            LOGI("APP_CMD_RESUME");
            break;
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            m_IsVisible = true;
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            LOGI("APP_CMD_WINDOW_RESIZED");
            break;
        default:
            LOGI("OnAppCommand: (unknown command)");
            break;
    }
}

int32_t App::OnInputEvent(android_app* androidApp, AInputEvent* event) {
/*
Engine* eng = (Engine*)app->userData;
if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
ndk_helper::GESTURE_STATE doubleTapState =
eng->doubletap_detector_.Detect(event);
ndk_helper::GESTURE_STATE dragState = eng->drag_detector_.Detect(event);
ndk_helper::GESTURE_STATE pinchState = eng->pinch_detector_.Detect(event);

// Double tap detector has a priority over other detectors
if (doubleTapState == ndk_helper::GESTURE_STATE_ACTION) {
// Detect double tap
eng->tap_camera_.Reset(true);
} else {
// Handle drag state
if (dragState & ndk_helper::GESTURE_STATE_START) {
// Otherwise, start dragging
ndk_helper::Vec2 v;
eng->drag_detector_.GetPointer(v);
eng->TransformPosition(v);
eng->tap_camera_.BeginDrag(v);
} else if (dragState & ndk_helper::GESTURE_STATE_MOVE) {
ndk_helper::Vec2 v;
eng->drag_detector_.GetPointer(v);
eng->TransformPosition(v);
eng->tap_camera_.Drag(v);
} else if (dragState & ndk_helper::GESTURE_STATE_END) {
eng->tap_camera_.EndDrag();
}

// Handle pinch state
if (pinchState & ndk_helper::GESTURE_STATE_START) {
// Start new pinch
ndk_helper::Vec2 v1;
ndk_helper::Vec2 v2;
eng->pinch_detector_.GetPointers(v1, v2);
eng->TransformPosition(v1);
eng->TransformPosition(v2);
eng->tap_camera_.BeginPinch(v1, v2);
} else if (pinchState & ndk_helper::GESTURE_STATE_MOVE) {
// Multi touch
// Start new pinch
ndk_helper::Vec2 v1;
ndk_helper::Vec2 v2;
eng->pinch_detector_.GetPointers(v1, v2);
eng->TransformPosition(v1);
eng->TransformPosition(v2);
eng->tap_camera_.Pinch(v1, v2);
}
}
return 1;
}
*/
    return 1;
}
#endif

#ifdef BUILD_DESKTOP
void App::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void App::SetFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    App* app = App::GetInstance();
    app->UpdateWindowSize(width, height);
}
#endif

void App::UpdateWindowSize(int width, int height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;
    m_Renderer->UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);
}

void App::CalculateFrameStats() {
    static int frameCount = 0;
    static float timeElapsed = 0.0f;

    frameCount++;
    timeElapsed += m_GlobalTimer->GetDelta();

    if (timeElapsed >= 1000.0f)
    {
        float fps = frameCount * 1000.f / timeElapsed;

        // Log
        static char buffer[100];
        sprintf(buffer, "FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTotalTime(), frameCount);
#ifdef BUILD_ANDROID
        JNIUtil::GetInstance()->LogTrigger(buffer);
#endif
        LOGI("FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTotalTime(), frameCount);

        frameCount = 0;
        timeElapsed -= 1000.0f;
    }
}
