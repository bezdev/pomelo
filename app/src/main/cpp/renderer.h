#pragma once

#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

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
        LOGD("Renderer::GetInstance()");
        return instance;
    };

    Renderer();
    ~Renderer();

    int Initialize(android_app* app);
    void LoadShaders();

    void AddRenderObject(Model* model);
    bool IsInitialized() { return m_IsInitialized; };

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
