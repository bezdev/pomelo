#pragma once

#include <jni.h>
#include <errno.h>
#include <vector>
#include <mutex>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include "renderer.h"
#include "timer.h"
#include "util.h"

#define NATIVE_ACTIVITY_CLASS_NAME "android/app/NativeActivity"

#define DEBUG 1

class Renderer;

class App {
public:
    App();
    ~App();

    static App* GetInstance()
    {
        static App* instance = new App();
        if (instance == nullptr) LOGD("YEP TRUE");
        else LOGD("YEP FALSE");
        return instance;
    };

    bool IsReady();
    android_app* GetApp() { return m_App; }

    void Initialize(android_app*);
    void Run();
    void OnAppCommand(android_app*, int32_t);

    std::vector<char> ReadFile(const char*);
private:
    void CalculateFrameStats();

    static void DetachCurrentThreadDtor(void* p) {
        LOGI("detached current thread");
        if (p != nullptr) {
            ANativeActivity *activity = (ANativeActivity *) p;
            activity->vm->DetachCurrentThread();
        }
    }

    JNIEnv* AttachCurrentThread();
    void DetachCurrentThread();
    jstring GetExternalFilesDirectory(JNIEnv*);
    mutable std::mutex m_ActivityMutex;

    android_app* m_App;
    Renderer* m_Renderer;
    Timer* m_GlobalTimer;
    bool m_HasFocus;
    bool m_IsVisible;
    bool m_HasWindow;
    bool m_IsReady;
    static void OnAppCmd(struct android_app*, int32_t);
    static int32_t OnInputEvent(android_app*, AInputEvent*);
};
