#include "renderer.h"
#include "shader.h"

#include "InputManager.h"

#include "jniutil.h"
#include "buffers.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer():
    m_IsInitialized(false),
    m_ScreenWidth(0),
    m_ScreenHeight(0),
    m_IsDrawWireFrame(false)
{
    InputManager::GetInstance()->RegisterCallback(InputEvent::KEY_W, [&](InputEvent event, InputData data) {
        if (data.Action == InputAction::UP)
        {
            m_IsDrawWireFrame = !m_IsDrawWireFrame;
        }
    });
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
        GLint minorVersion;
        glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
        glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
        LOGI("OpenGL Version: %d.%d", majorVersion, minorVersion);

        if (majorVersion == 3 && minorVersion < 3)
        {
            bool hasARBDrawInstanced = false;
            std::string extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
            std::stringstream ss(extensions);
            std::string extension;
            LOGI("glGetString(GL_EXTENSIONS): %s", extensions.c_str());
            while (ss >> extension)
            {
                if (extension == "GL_ARB_draw_instanced")
                {
                    hasARBDrawInstanced = true;
                }

                // LOGI(extension.c_str());
            }

            if (majorVersion == 3 && minorVersion < 3 && !hasARBDrawInstanced)
            {
                LOGE("no instancing support");
            }
        }

        m_IsInitialized = true;
    }

    return 0;
}

void Renderer::Cleanup()
{

}

void Renderer::LoadEntities(const std::vector<Entity>& entities)
{
    m_ShaderManager.Cleanup();
    m_RenderBufferManager.Cleanup();
    m_RenderQueue.clear();

    std::map<Components::MeshType, std::vector<const Entity*>> instancedMap;

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

            if (mesh->Type == Components::MeshType::INSTANCED_BOX)
            {
                instancedMap[mesh->Type].push_back(&entity);
                continue;
            }
        }

        if (material != nullptr && mesh != nullptr)
        {
            if (material->Type == Components::MaterialType::SOLID_COLOR)
            {
                RenderObject ro;
                ro.RenderBuffer = m_RenderBufferManager.GetRenderBuffer(mesh->Type);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR);
                ro.Entity = const_cast<Entity*>(&entity);
                ro.Material = material;
                ro.Mesh = mesh;
                ro.Transform = &entity.GetComponent<Components::Transform>();
                m_RenderQueue.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::PIXEL_COLOR && mesh->Type == Components::MeshType::AXIS)
            {
                RenderObject ro;
                ro.RenderBuffer = m_RenderBufferManager.GetRenderBuffer(Components::MeshType::AXIS);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
                ro.Entity = const_cast<Entity*>(&entity);
                ro.Material = material;
                ro.Mesh = mesh;
                ro.Transform = &entity.GetComponent<Components::Transform>();
                m_RenderQueue.push_back(ro);
            }
        }
    }

    for (const auto& kv : instancedMap)
    {
        std::vector<glm::vec3> positions(kv.second.size());

        for (const auto& e : kv.second)
        {
            auto& position = e->GetComponent<Components::Transform>();
            positions.push_back(position.Position);
        }

        RenderObject ro;
        ro.RenderBuffer = m_RenderBufferManager.CreateInstancedBox(positions);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR_INSTANCED);
        // ro.Entity = const_cast<Entity*>();
        // ro.Material = material;
        // ro.Mesh = mesh;
        // ro.Transform = &entity.GetComponent<Components::Transform>();
        m_RenderQueue.push_back(ro);
    }
    // Create instanced buffers
    // Create shader

    std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), [](RenderObject a, RenderObject b)
    {
        if (a.Material->Type != b.Material->Type)
        {
            return a.Material->Type < b.Material->Type;
        }
        else
        {
            return a.Mesh->Type < b.Mesh->Type;
        }
    });
}

void Renderer::UpdateWindowSize(int width, int height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;

    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

    Camera::GetInstance()->UpdateViewSize(width, height);
}

void Renderer::Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Shader* currentShader = nullptr;
    Components::Material* currentMaterial = nullptr;
    RenderBuffer* currentRenderBuffer = nullptr;

    for (RenderObject& ro : m_RenderQueue)
    {
        Shader* shader = ro.Shader;
        Entity* entity = ro.Entity;
        Components::Material* material = ro.Material;
        RenderBuffer* renderBuffer = ro.RenderBuffer;

        if (shader != currentShader)
        {
            shader->Use();

            glm::mat4 viewMatrix = Camera::GetInstance()->GetViewMatrix();
            glm::mat4 projectionMatrix = Camera::GetInstance()->GetProjectionMatrix();
            shader->SetVPMatrix(
                glm::value_ptr(viewMatrix),
                glm::value_ptr(projectionMatrix));

            currentShader = shader;
        }

        if (renderBuffer != currentRenderBuffer)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            ro.RenderBuffer->VAO->Bind();
            ro.RenderBuffer->IBO->Bind();

            currentRenderBuffer = renderBuffer;
        }

        shader->SetPerEntity(entity);

        if (currentMaterial != material)
        {
            shader->SetPerMaterial(material);

            currentMaterial = material;
        }

        if (m_IsDrawWireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        shader->Draw(renderBuffer);

        if (m_IsDrawWireFrame)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}
