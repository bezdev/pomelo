#include <jni.h>
#include <errno.h>

#include <string>
#include <vector>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include "app.h"
#include "scene.h"

struct android_app;

void android_main(struct android_app* android_app) {
    App* app = App::GetInstance();
    app->Initialize(android_app);
    app->Run();
}

/*
extern "C" JNIEXPORT jstring
extern "C" JNICALL Java_com_bezdev_fame_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
*/
