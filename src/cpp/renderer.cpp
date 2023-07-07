#include "renderer.h"
#include "shader.h"
#include "jniutil.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer():
    m_IsInitialized(false),
    m_ScreenWidth(0),
    m_ScreenHeight(0)
{
}

Renderer::~Renderer()
{
    LOGD("Renderer::~Renderer");
    Cleanup();
}

int Renderer::Initialize()
{
    if (!m_IsInitialized)
    {
        GLint majorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        GLint minorVersion;
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
        LOGI("OpenGL Version: %d.%d", majorVersion, minorVersion);

        // TODO: do this dynamically
        LoadShaders();
        Meshes::Box b(1.f, 1.f, 1.f);
        m_RenderBuffers[static_cast<int>(Components::MeshType::BOX)] = CreateRenderBuffer(b.Vertices, b.Indices, GL_TRIANGLES);

        m_IsInitialized = true;
    }

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
        Shader::CompileShader("shaders/SolidColor.vs", Util::ReadFile("shaders/SolidColor.vs"), GL_VERTEX_SHADER),
        Shader::CompileShader("shaders/SolidColor.fs", Util::ReadFile("shaders/SolidColor.fs"), GL_FRAGMENT_SHADER)),
        variables);
}

void Renderer::Cleanup() {

}

void Renderer::LoadEntities(const std::vector<Entity>& entities)
{
    std::vector<RenderObject> renderObjects;

    for (const Entity& entity : entities)
    {
        Components::Material* material = nullptr;
        Components::Mesh* mesh = nullptr;

        if (entity.HasComponent<Components::Material>())
        {
            material = &entity.GetComponent<Components::Material>();
        }

        if (entity.HasComponent<Components::Mesh>())
        {
            mesh = &entity.GetComponent<Components::Mesh>();
        }

        if (material->Type == Components::MaterialType::SolidColor && mesh->Type == Components::MeshType::BOX)
        {
            RenderObject ro;
            ro.RenderBuffer = m_RenderBuffers[static_cast<int>(Components::MeshType::BOX)];
            ro.Shader = Shader::SOLID_COLOR_SHADER;
            ro.Entity = &entity;
            ro.Material = material;
            ro.Mesh = mesh;
            ro.Motion = &entity.GetComponent<Components::Motion>();
            renderObjects.push_back(ro);
        }
    }

    for (auto ro : renderObjects)
    {
        m_RenderQueue.push_back(ro);
    }
}

void Renderer::UpdateWindowSize(int width, int height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;

    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

    auto ratio = 1.f * m_ScreenWidth / m_ScreenHeight;
    m_ProjectionMatrix = glm::perspective(glm::radians<float>(45.0f), ratio, 3.f, 500.f);
}

void Renderer::Render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto viewMatrix = glm::lookAt(glm::vec3(10, 10, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    static const  Shader* currentShader = nullptr;
    static const Components::Material* currentMaterial = nullptr;
    static Components::Mesh* currentMesh = nullptr;
    static GLuint currentPositionHandle = 0;
    static GLuint currentColorHandle = 0;
    static GLuint currentModelMatrixHandle = 0;
    static GLuint currentViewMatrixHandle = 0;
    static GLuint currentProjectionMatrixHandle = 0;
    for (auto ro : m_RenderQueue) {
        const Shader* shader = ro.Shader;
        const Components::Material* material = ro.Material;
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

            glUniformMatrix4fv(currentViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(currentProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
        }

        // auto rb = m_RenderBuffers[static_cast<int>(Components::MeshType::BOX)];
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        ro.RenderBuffer->VAO->Bind();
        ro.RenderBuffer->IBO->Bind();

        auto position = ro.Entity->GetComponent<Components::Motion>();
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), position.Position);
        glUniformMatrix4fv(currentModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        // Per object properties
        if (currentMaterial != material) {
            currentMaterial = material;

            if (currentMaterial->Type == Components::MaterialType::SolidColor) {
                // TODO: replace with actual color from material
                glUniform4f(currentColorHandle, 0.2f, 0.709803922f, 0.898039216f, 1.0f);
            }
        }

        glDrawElements(GL_TRIANGLES, ro.RenderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
    }
}
