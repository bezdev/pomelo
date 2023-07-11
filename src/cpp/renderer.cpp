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
        // TODO: hack
        Meshes::Axis axis(1.f, 1.f, 1.f);
        RenderBuffer *rb = new RenderBuffer();
        rb->VAO = new VertexArray();
        rb->VAO->Bind();
        rb->VAO->AddVertexBuffer(new VertexBuffer(&axis.Vertices[0], axis.Vertices.size(), sizeof(GLfloat), 3, 0));
        rb->VAO->AddVertexBuffer(new VertexBuffer(&axis.Colors[0], axis.Colors.size(), sizeof(GLfloat), 4, 1));
        rb->VAO->Unbind();
        rb->IBO = new IndexBuffer(reinterpret_cast<GLushort *>(&axis.Indices[0]), axis.Indices.size() * sizeof(GLushort));
        m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)] = rb;

        m_IsInitialized = true;

        LOGI("glGetString(GL_EXTENSIONS): %s", glGetString(GL_EXTENSIONS));
    }

    return 0;
}

void Renderer::LoadShaders()
{
    Shader::SOLID_COLOR_SHADER = new SolidColorShader();
    Shader::PIXEL_COLOR_SHADER = new PixelColorShader();
}

void Renderer::Cleanup()
{

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
            ro.Entity = const_cast<Entity*>(&entity);
            ro.Material = material;
            ro.Mesh = mesh;
            ro.Motion = &entity.GetComponent<Components::Motion>();
            renderObjects.push_back(ro);
        }
        else if (material->Type == Components::MaterialType::PixelColor && mesh->Type == Components::MeshType::AXIS)
        {
            RenderObject ro;
            ro.RenderBuffer = m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)];
            ro.Shader = Shader::PIXEL_COLOR_SHADER;
            ro.Entity = const_cast<Entity*>(&entity);
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

    m_ProjectionMatrix = glm::frustum<float>(-ratio, ratio, -1.0f, 1.0f, 3, 500);
}

void Renderer::Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto viewMatrix = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    static Shader* currentShader = nullptr;
    static const Components::Material* currentMaterial = nullptr;
    static const RenderBuffer* currentRenderBuffer = nullptr;
    static Components::Mesh* currentMesh = nullptr;
    static GLuint currentPositionHandle = 0;
    static GLuint currentColorHandle = 0;
    static GLuint currentModelMatrixHandle = 0;
    static GLuint currentViewMatrixHandle = 0;
    static GLuint currentProjectionMatrixHandle = 0;
    for (auto ro : m_RenderQueue)
    {
        Shader* shader = ro.Shader;
        const Components::Material* material = ro.Material;
        const RenderBuffer* renderBuffer = ro.RenderBuffer;

        if (shader != currentShader)
        {
            shader->Use();
            shader->SetVPMatrix(glm::value_ptr(viewMatrix), glm::value_ptr(m_ProjectionMatrix));
            currentShader = shader;
        }

        if (renderBuffer != currentRenderBuffer)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            if (renderBuffer == m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)])
            {
                glLineWidth(20);
            }

            ro.RenderBuffer->VAO->Bind();
            ro.RenderBuffer->IBO->Bind();

            currentRenderBuffer = renderBuffer;
        }

        shader->SetPerEntity(ro.Entity);

        if (currentMaterial != material) {
            shader->SetPerMaterial(material);

            currentMaterial = material;
        }

        if (shader == Shader::SOLID_COLOR_SHADER) glDrawElements(GL_TRIANGLES, ro.RenderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
        else glDrawElements(GL_LINES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
    }
}
