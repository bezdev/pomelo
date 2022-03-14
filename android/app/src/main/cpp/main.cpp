#include <jni.h>
#include <errno.h>

#include <memory>
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
    LOGD("android_app %d", android_app->window);
    //std::unique_ptr<App> app(App::GetInstance());
    App* app = App::GetInstance();
    app->Initialize(android_app);
    app->SetStartScene(SCENE_CUBE);
    app->Run();
    delete app;
    app = nullptr;
    LOGD("end main");
}

// TODO:
// 1) SceneManager (load/unload scenes)
// 2) Renderer support to not render until shit is loaded
// 3) Call Logger from c++ like above
// 4) Call Scene from java
// 5) Test only flags
