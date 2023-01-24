#include "AndroidApp.h"

AndroidApp* AndroidApp::Instance = nullptr;

AndroidApp::AndroidApp(android_app *androidApp):
    m_AndroidApp(androidApp),
    m_HasFocus(false)
{
    m_AndroidApp->userData = this;
    m_AndroidApp->onAppCmd = AndroidApp::OnAppCmd;
    m_AndroidApp->onInputEvent = AndroidApp::OnInputEvent;

    m_App.SetStartScene(SCENE_CUBE);

    Instance = this;
}

AndroidApp::~AndroidApp()
{
    if (m_Display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (m_Surface != EGL_NO_SURFACE) {
            eglDestroySurface(m_Display, m_Surface);
        }

        if (m_Context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_Display, m_Context);
        }

        eglTerminate(m_Display);
    }
}

void AndroidApp::InitializeWindow(ANativeWindow *window)
{
    m_Window = window;
    m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(m_Display, 0, 0);

    const EGLint attributes[] =
    {
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_RED_SIZE,
        8,
        EGL_DEPTH_SIZE,
        24,
        EGL_NONE
    };

    EGLint configCount;
    eglChooseConfig(m_Display, attributes, &m_Config, 1, &configCount);

    THROW_IF_FALSE(configCount, "eglChooseConfig failed");

    m_Surface = eglCreateWindowSurface(m_Display, m_Config, m_Window, NULL);
    //eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &m_ScreenWidth);
    //eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &m_ScreenHeight);

    const EGLint contextAttributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION,
        3,
        EGL_NONE
    };
    m_Context = eglCreateContext(m_Display, m_Config, NULL, contextAttributes);

    THROW_IF_FALSE(eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context), "eglMakeCurrent failed");
}

void AndroidApp::OnAppCmd(struct android_app* androidApp, int32_t command) {
    // TDOO: static cast?
    AndroidApp* aa = (AndroidApp*)androidApp->userData;
    aa->OnAppCommand(androidApp, command);
}

void AndroidApp::OnAppCommand(android_app* androidApp, int32_t command) {
    AndroidApp* aa = (AndroidApp*)androidApp->userData;
    switch (command) {
        case APP_CMD_SAVE_STATE:
            LOGI("APP_CMD_SAVE_STATE");
            break;
        case APP_CMD_INIT_WINDOW:
            LOGI("APP_CMD_INIT_WINDOW");
            InitializeWindow(androidApp->window);
            m_App.Initialize();

            int32_t width, height;
            eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &width);
            eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &height);
            m_App.UpdateWindowSize(width, height);
            SetFocus(true);

            break;
        case APP_CMD_TERM_WINDOW:
            LOGI("APP_CMD_TERM_WINDOW");
            // TODO: close app
            aa->SetFocus(false);
            break;
        case APP_CMD_GAINED_FOCUS:
            LOGI("APP_CMD_GAINED_FOCUS");
            aa->SetFocus(true);
            break;
        case APP_CMD_LOST_FOCUS:
            LOGI("APP_CMD_LOST_FOCUS");
            aa->SetFocus(false);
            break;
        case APP_CMD_LOW_MEMORY:
            LOGI("APP_CMD_LOW_MEMORY");
            break;
        case APP_CMD_STOP:
            LOGI("APP_CMD_STOP");
            break;
        case APP_CMD_PAUSE:
            LOGI("APP_CMD_PAUSE");
            break;
        case APP_CMD_RESUME:
            LOGI("APP_CMD_RESUME");
            break;
        case APP_CMD_START:
            LOGI("APP_CMD_START");
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            LOGI("APP_CMD_WINDOW_RESIZED");
            break;
        case APP_CMD_INPUT_CHANGED:
            LOGI("APP_CMD_INPUT_CHANGED");
            break;
        default:
            LOGI("OnAppCommand Unhandled Command: %d", command);
            break;
    }
}

int32_t AndroidApp::OnInputEvent(android_app* androidApp, AInputEvent* event) {
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

void AndroidApp::Run()
{
    while(true) {
        int id;
        int events;
        android_poll_source *source;
        bool isReady;
        while ((id = ALooper_pollAll((isReady = IsReady()) ? 0 : -1, NULL, &events, (void **) &source)) >= 0) {
            if (source != NULL) source->process(m_AndroidApp, source);

            if (m_AndroidApp->destroyRequested != 0) {
                LOGI("AndroidApp::Run Destroy Requested");
                return;
            }
        }

        if (!isReady) continue;

        m_App.Run();

        eglSwapBuffers(m_Display, m_Surface);
//        if (isFirstFrame) {
//            LOGD("first frame");
//            m_Renderer->Initialize();
//
//            glfwGetFramebufferSize(m_Window, &m_ScreenWidth, &m_ScreenHeight);
//            UpdateWindowSize(m_ScreenWidth, m_ScreenHeight);
//
//            // TODO: creation of meshes should happen dynamically
//            Mesh::CreateBoxMesh(1.f, 1.f, 1.f);
//
//            SceneManager::LoadScene(m_StartSceneId);
//
//            m_GlobalTimer->Reset();
//            isFirstFrame = false;
//        } else {
//            m_GlobalTimer->Update();
//        }
//
//        m_Renderer->Render();

        //LogFPS();
    }
}
