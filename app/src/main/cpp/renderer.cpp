#include <EGL/egl.h>

#include "renderer.h"
#include "shader.h"
#include "jniutil.h"

Renderer::Renderer():
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
    LOGD("Renderer::~Renderer");
    Cleanup();
}

int Renderer::Initialize(android_app* app)
{
    if (!m_IsInitialized)
    {
        _Initialize(app->window);
        //JNIUtil::GetInstance()->Initialize(app->activity, m_App); TODO NOT NEEDED?

        LoadShaders();

        m_IsInitialized = true;
    }
    else if(app->window != m_Window)
    {
        THROW("not implemented 1");
    }
    else
    {
        THROW("not implemented 2");
    }

    return 0;
}

int Renderer::_Initialize(ANativeWindow* window)
{
    m_Window = window;
    m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(m_Display, 0, 0);

    const EGLint attributes[] =
    {
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

    const EGLint contextAttributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION,
        3,
        EGL_NONE
    };
    m_Context = eglCreateContext(m_Display, m_Config, NULL, contextAttributes);

    THROW_IF_FALSE(eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context), "eglMakeCurrent failed");

    return 0;
}

void Renderer::LoadShaders() {
    std::vector<ShaderVariable> variables
    {
        { ShaderVariableType::ATTRIBUTE, "vPosition" },
        { ShaderVariableType::UNIFORM, "vColor" },
        { ShaderVariableType::UNIFORM, "modelMatrix" },
        { ShaderVariableType::UNIFORM, "viewMatrix" },
        { ShaderVariableType::UNIFORM, "projectionMatrix" },
    };

    Shader::SOLID_COLOR_SHADER = new Shader(Shader::LinkShader(
        Shader::CompileShader(App::GetInstance()->ReadFile("shaders/SolidColor.vs"), GL_VERTEX_SHADER),
        Shader::CompileShader(App::GetInstance()->ReadFile("shaders/SolidColor.fs"), GL_FRAGMENT_SHADER)),
        variables);

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
    //m_RenderObjectCollection.Add(model);
    m_RenderObjects.push_back(model);
}

void Renderer::Render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
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

    auto viewMatrix = glm::lookAt(glm::vec3(10, 10, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    auto ratio = 1.f * m_ScreenWidth / m_ScreenHeight;
    auto projectionMatrix = glm::frustum(-ratio, ratio, -1.f, 1.f, 3.f, 500.f);
    //auto projectionMatrix = glm::perspective(45, ratio, 3, 500);

    static Shader* currentShader = nullptr;
    static Material* currentMaterial = nullptr;
    static Mesh* currentMesh = nullptr;
    static GLuint currentPositionHandle = 0;
    static GLuint currentColorHandle = 0;
    static GLuint currentModelMatrixHandle = 0;
    static GLuint currentViewMatrixHandle = 0;
    static GLuint currentProjectionMatrixHandle = 0;
    int objectCount = 0;
    //for (auto itByMaterialType = m_RenderObjectCollection.GetRenderObjects().begin(); itByMaterialType != m_RenderObjectCollection.GetRenderObjects().end(); itByMaterialType++) {
    //    for (auto itByMeshType = itByMaterialType->second.begin(); itByMeshType != itByMaterialType->second.end(); itByMeshType++) {
    //        for (auto itModels = itByMeshType->second.begin(); itModels != itByMeshType->second.end(); itModels++) {
    for (auto itModels = m_RenderObjects.begin(); itModels != m_RenderObjects.end(); itModels++) {
                Model* model = *itModels;
                Material* material = model->GetMaterial();
                Shader* shader = material->GetShader();
                Mesh* mesh = model->GetMesh();
                if (shader != currentShader) {
                    glUseProgram(shader->GetProgram());
                    currentShader = shader;

                    // TODO: support more shaders
                    auto variables = currentShader->GetVariables();
                    currentPositionHandle = variables[0];
                    currentColorHandle = variables[1];
                    currentModelMatrixHandle = variables[2];
                    currentViewMatrixHandle = variables[3];
                    currentProjectionMatrixHandle = variables[4];

                    glUniformMatrix4fv(currentViewMatrixHandle, 1, false, glm::value_ptr(viewMatrix));
                    glUniformMatrix4fv(currentProjectionMatrixHandle, 1, false, glm::value_ptr(projectionMatrix));
                }

                if (mesh != currentMesh) {
                    // TODO: make this better
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

                    currentMesh = mesh;
                    if (currentMesh == Mesh::PRIMITIVE_BOX_TRIANGLE) {
                        currentMesh->GetVertexBuffer()->BindBuffer();
                        currentMesh->GetIndexBuffer()->BindBuffer();
                        glVertexAttribPointer(currentPositionHandle, 3, GL_FLOAT, false, 3 * 4, 0);
                        glEnableVertexAttribArray(currentPositionHandle);
                    }
                }

                glUniformMatrix4fv(currentModelMatrixHandle, 1, false, glm::value_ptr(glm::mat4(1.f)));

                // Per object properties
                if (currentMaterial != material) {
                    currentMaterial = material;

                    if (currentMaterial->GetType() == MaterialType::SolidColor) {
                        // TODO: replace with actual color from material
                        glUniform4f(currentColorHandle, 0.2f, 0.709803922f, 0.898039216f, 1.0f);
                    }
                }

                glDrawElements(GL_TRIANGLES, currentMesh->GetIndexBuffer()->GetCount(), GL_UNSIGNED_SHORT, 0);
            }
   //     }
   // }

    eglSwapBuffers(m_Display, m_Surface);
}
