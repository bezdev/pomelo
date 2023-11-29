#pragma once

#include <thread>

#ifdef BUILD_ANDROID
#include <android_native_app_glue.h>
#include <jni.h>

#include "app/App.h"
#include "util/Timer.h"

class JNIUtil
{
public:
    static JNIUtil* GetInstance()
    {
        static JNIUtil* instance = new JNIUtil();
        LOGD("JNIUtil::GetInstance()");
        return instance;
    }

    //void Initialize(ANativeActivity* activity, App* app);
    void Initialize(App* app);
    JNIEnv* AttachCurrentThread();
    void DetachCurrentThread();

    void LogTrigger(const char* message);
    void Log(const char* message);

    ~JNIUtil();
private:
    JNIUtil();

    App* m_App;
    ANativeActivity* m_Activity;
    std::thread m_JNILogThread;
};
#endif
