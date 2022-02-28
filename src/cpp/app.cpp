#include "app.h"
#include "jniutil.h"

App::App() :
    m_HasFocus(false),
    m_IsReady(false),
    m_HasWindow(false),
    m_IsVisible(false)
    { 
        LOGD("App::App");
        LOGD("Timer: %p", m_GlobalTimer);
    }


App::~App() {
    LOGD("App::~App");

    delete Renderer::GetInstance();
    delete JNIUtil::GetInstance();
    delete m_GlobalTimer;
}

bool App::IsReady() {
    return m_HasFocus && m_HasWindow && m_IsVisible;
}

void App::Initialize(android_app* androidApp) {
    m_App = androidApp;
    m_App->userData = this;
    m_App->onAppCmd = App::OnAppCmd;
    m_App->onInputEvent = App::OnInputEvent;

    m_GlobalTimer = new Timer();
    m_Renderer = Renderer::GetInstance();
    //JNIUtil::GetInstance()->Initialize(androidApp->activity, this);
    JNIUtil::GetInstance()->Initialize(this);
}

void App::Run() {
    bool isReady;
    bool isFirstFrame = true;

    while (true) {
        int id;
        int events;
        android_poll_source* source;
        bool shouldDestroy = false;
        while ((id = ALooper_pollAll((isReady = IsReady()) ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
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

        if (isFirstFrame) {
            LOGD("first frame");
            m_Renderer->Initialize(m_App);

            // TODO: Scene creation should happen somewhere else
            Mesh::CreateBoxMesh(1.f, 1.f, 1.f);
            Scene::CreateCubeScene();

            m_GlobalTimer->Reset();
            isFirstFrame = false;
        } else {
            m_GlobalTimer->Update();
        }

        m_Renderer->Render();

        CalculateFrameStats();
    }
}

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

std::vector<char> App::ReadFile(const char* filename) {
    std::vector<char> data;

#if 0
    if (m_App->activity == nullptr) {
        THROW("Activity is null");
    }

    std::lock_guard<std::mutex> lock(m_ActivityMutex);

    JNIEnv* env = AttachCurrentThread();
    jstring jPath = GetExternalFilesDirectory(env);

    std::string s;
    if (jPath) {
        const char* path = env->GetStringUTFChars(jPath, NULL);
        s = std::string(path);
        if (filename[0] != '/') {
            s.append("/");
        }
        s.append(filename);
        env->ReleaseStringUTFChars(jPath, path);
        env->DeleteLocalRef(jPath);
    }

    LOGD("PATH: %s", s.c_str());
    DetachCurrentThread();
#endif
    AAssetManager* assetManager = m_App->activity->assetManager;
    AAsset* assetFile = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
    if (!assetFile) {
        LOGE("file does not exist: %s", filename);
        THROW_FILE_NOT_FOUND();
    }

    char* rawData = (char*)AAsset_getBuffer(assetFile);
    int32_t size = AAsset_getLength(assetFile);
    if (rawData == NULL) {
        AAsset_close(assetFile);
        LOGE("error reading file: %s", filename);
    }

    data.reserve(size);
    data.assign(rawData, rawData + size);

    AAsset_close(assetFile);

    return data;
}

JNIEnv* App::AttachCurrentThread() {
    JNIEnv* env;
    if (m_App->activity->vm->GetEnv((void**)&env, JNI_VERSION_1_4) == JNI_OK)
        return env;
    m_App->activity->vm->AttachCurrentThread(&env, NULL);
    /*
    pthread_key_t key;
    if (pthread_key_create(&key, DetachCurrentThreadDtor) == 0) {
        pthread_setspecific(key, (void *)m_App->activity);
    }
    */
    return env;
}

void App::DetachCurrentThread() {
    m_App->activity->vm->DetachCurrentThread();
}

jstring App::GetExternalFilesDirectory(JNIEnv* env) {
    jstring path = nullptr;
    
    // Invoking getExternalFilesDir() java API
    jclass activityClass = env->FindClass(NATIVE_ACTIVITY_CLASS_NAME);
    jmethodID mID = env->GetMethodID(activityClass, "getExternalFilesDir", "(Ljava/lang/String;)Ljava/io/File;");
    jobject fileObject = env->CallObjectMethod(m_App->activity->clazz, mID, NULL);
    if (fileObject) {
        jclass fileClass = env->FindClass("java/io/File");
        mID = env->GetMethodID(fileClass, "getPath", "()Ljava/lang/String;");
        path = (jstring)env->CallObjectMethod(fileObject, mID);
    }

    return path;
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
        JNIUtil::GetInstance()->LogTrigger(buffer);
        LOGI("FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTotalTime(), frameCount);

        frameCount = 0;
        timeElapsed -= 1000.0f;
    }
}
