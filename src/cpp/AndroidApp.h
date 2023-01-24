#pragma once

#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "app.h"
#include "util.h"

class AndroidApp
{
public:
    static AndroidApp* Instance;

    static void OnAppCmd(struct android_app*, int32_t);
    static int32_t OnInputEvent(android_app*, AInputEvent*);

    AndroidApp(android_app* androidApp);
    ~AndroidApp();

    void InitializeWindow(ANativeWindow* window);
    void OnAppCommand(android_app*, int32_t);

    void Run();

    void SetFocus(bool focus) { m_HasFocus = focus; };
    bool IsReady() { return m_HasFocus; };

    android_app* GetAndroidApp() { return m_AndroidApp; }

private:
    bool m_HasFocus;

    App m_App;
    android_app* m_AndroidApp;
    ANativeWindow* m_Window;
    EGLDisplay m_Display;
    EGLConfig m_Config;
    EGLSurface m_Surface;
    EGLContext m_Context;
};