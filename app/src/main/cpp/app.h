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

#define NATIVEACTIVITY_CLASS_NAME "android/app/NativeActivity"

#define DEBUG 1

class Renderer;

class App {
public:
    static App* GetInstance() {
        static App* s_App = nullptr;
        if (s_App == nullptr) {
            s_App = new App();
        }
        return s_App;
    };

    App();
    ~App();

    void Run();
    void Initialize(android_app*);
    bool IsReady();

    std::vector<char> ReadFile(const char* filename);
private:
    void CalculateFrameStats() {
        static int frameCount = 0;
        static float timeElapsed = 0.0f;

        frameCount++;
        timeElapsed += m_GlobalTimer->GetDelta();

        if (timeElapsed >= 1000.0f)
        {
            float fps = frameCount * 1000.f / timeElapsed;
            LOGI("FPS: %.4f - Total Time (ms): %0.f Frames: %d", fps, m_GlobalTimer->GetTime(), frameCount);

            frameCount = 0;
            timeElapsed -= 1000.0f;
        }
    }

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
    bool m_IsReady;
    static void OnAppCmd(struct android_app*, int32_t);
    static int32_t OnInputEvent(android_app*, AInputEvent*);
};
