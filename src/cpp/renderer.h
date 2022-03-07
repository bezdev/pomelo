#pragma once

#ifdef BUILD_ANDROID
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#ifdef BUILD_DESKTOP
#define EGL_NO_DISPLAY 0
#define EGL_NO_SURFACE 0
#define EGL_NO_CONTEXT 0

typedef int EGLDisplay;
typedef int EGLConfig;
typedef int EGLSurface;
typedef int EGLContext;
typedef int ANativeWindow;
typedef int android_app;
#endif

#include "app.h"
#include "model.h"
#include "renderobjectcollection.h"
#include "scene.h"
#include "util.h"

// TODO: move to other file
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class App;

class Renderer {
public:
    static Renderer* GetInstance()
    {
        static Renderer* instance = new Renderer();
        //LOGD("Renderer::GetInstance()");
        return instance;
    };

    Renderer();
    ~Renderer();

    int Initialize();
    void LoadShaders();

    bool IsInitialized() { return m_IsInitialized; };
    void AddRenderObject(Model* model);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    bool m_IsInitialized;
    ANativeWindow* m_Window;
    EGLDisplay m_Display;
    EGLConfig m_Config;
    EGLSurface m_Surface;
    EGLContext m_Context;

    int32_t m_ScreenWidth;
    int32_t m_ScreenHeight;

    //RenderObjectCollection m_RenderObjectCollection;
    std::vector<Model*> m_RenderObjects;

    int _Initialize(ANativeWindow* app);

    static void CheckGlError(const char* op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    void Cleanup();
};
