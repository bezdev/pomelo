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

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "fame-native", __VA_ARGS__))

struct android_app;

void android_main(struct android_app* state) {
    int i = 2;
    LOGI("OpenGL Info: %d", i);
    while (true) {
        i++;
    }

    return;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_bezdev_fame_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
