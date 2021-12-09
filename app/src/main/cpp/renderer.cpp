#include <EGL/egl.h>

#include "renderer.h"
#include "shader.h"

Renderer::Renderer():
    m_App(App::GetInstance()),
    m_IsInitialized(false),
    m_Window(nullptr),
    m_Display(EGL_NO_DISPLAY),
    m_Surface(EGL_NO_SURFACE),
    m_Context(EGL_NO_CONTEXT),
    m_ScreenWidth(0),
    m_ScreenHeight(0)
{
}

Renderer::~Renderer()
{
    Cleanup();
}

int Renderer::Initialize(android_app* app) {
    if (!m_IsInitialized) {
        _Initialize(app->window);
        LoadShaders();

        // TODO: Scene creation should happen somewhere else
        Mesh::CreateBoxMesh(1.f, 1.f, 1.f);
        Scene::CreateCubeScene();

        m_IsInitialized = true;
    } else if(app->window != m_Window) {
        THROW("not implemented");
    } else {
        THROW("not implemented");
    }

    return 0;
}

int Renderer::_Initialize(ANativeWindow* window) {
    m_Window = window;
    m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(m_Display, 0, 0);

    const EGLint attributes[] = {
        EGL_RENDERABLE_TYPE,
        EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE,
        EGL_WINDOW_BIT,
        EGL_BLUE_SIZE,
        8,
        EGL_GREEN_SIZE,
        8,
        EGL_RED_SIZE,
        8,
        EGL_DEPTH_SIZE,
        24,
        EGL_NONE
    };

    EGLint configCount;
    eglChooseConfig(m_Display, attributes, &m_Config, 1, &configCount);

    THROW_IF_FALSE(configCount, "eglChooseConfig failed");

    m_Surface = eglCreateWindowSurface(m_Display, m_Config, m_Window, NULL);
    //eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &m_ScreenWidth);
    //eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &m_ScreenHeight);

    const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION,
        3,
        EGL_NONE
    };
    m_Context = eglCreateContext(m_Display, m_Config, NULL, contextAttributes);

    THROW_IF_FALSE(eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context), "eglMakeCurrent failed");

    return 0;
}

void Renderer::LoadShaders() {
    Shader::SOLID_COLOR_SHADER = Shader::LinkShader(
        Shader::CompileShader(App::GetInstance()->ReadFile("shaders/SolidColor.vs"), GL_VERTEX_SHADER),
        Shader::CompileShader(App::GetInstance()->ReadFile("shaders/SolidColor.fs"), GL_FRAGMENT_SHADER));

    return;
}

void Renderer::Cleanup() {
    if (m_Display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (m_Surface != EGL_NO_SURFACE) {
            eglDestroySurface(m_Display, m_Surface);
        }

        if (m_Context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_Display, m_Context);
        }

        eglTerminate(m_Display);
    }
}

void Renderer::AddRenderObject(Model* model) {
    m_RenderObjectCollection.Add(model);
}

void Renderer::Render() {
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    int32_t width, height;
    eglQuerySurface(m_Display, m_Surface, EGL_WIDTH, &width);
    eglQuerySurface(m_Display, m_Surface, EGL_HEIGHT, &height);

    // TODO: make this a method
    if (width != m_ScreenWidth || height != m_ScreenHeight) {
        m_ScreenWidth = width;
        m_ScreenHeight = height;
        glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    }

    for (auto itByMaterialType = m_RenderObjectCollection.GetRenderObjects()->begin(); itByMaterialType != m_RenderObjectCollection.GetRenderObjects()->end(); itByMaterialType++) {
        for (auto itByMeshType = itByMaterialType->second.begin(); itByMeshType != itByMaterialType->second.end(); itByMeshType++) {
            for (auto itModels = itByMeshType->second.begin(); itModels != itByMeshType->second.end(); itModels++) {
                Model* model = *itModels;

            }
        }
    }

    eglSwapBuffers(m_Display, m_Surface);
}
