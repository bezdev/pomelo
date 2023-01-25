#include "AndroidApp.h"
#include "scene.h"

struct android_app;

void android_main(android_app* android_app) {
    LOGD("android_main start: %ld", android_app->window);
    AndroidApp* aa = AndroidApp::GetInstance();
    aa->Initialize(android_app);
    aa->Run();
    delete aa;
    LOGD("android_main end");
}

// TODO:
// 1) SceneManager (load/unload scenes)
// 2) Renderer support to not render until shit is loaded
// 3) Call Logger from c++ like above
// 4) Call Scene from java
// 5) Test only flags
