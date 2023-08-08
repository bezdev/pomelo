#include "renderer.h"
#include "shader.h"

#include "engine/InputManager.h"

#include "render/Buffers.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer():
    m_IsInitialized(false),
    m_ScreenWidth(0),
    m_ScreenHeight(0),
    m_IsDrawWireFrame(false)
{
    InputManager::GetInstance()->RegisterCallback(InputEvent::KEY_N, [&](InputEvent event, InputData data) {
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
    ScopeTimer s("LoadEntities");

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

            if (mesh->Type == Components::MeshType::INSTANCED_BOX || mesh->Type == Components::MeshType::LINE )
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
                ro.Entities.push_back(const_cast<Entity*>(&entity));
                m_RenderQueue.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::PIXEL_COLOR && mesh->Type == Components::MeshType::AXIS)
            {
                RenderObject ro;
                ro.RenderBuffer = m_RenderBufferManager.GetRenderBuffer(Components::MeshType::AXIS);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
                ro.Entities.push_back(const_cast<Entity*>(&entity));
                m_RenderQueue.push_back(ro);
            }
        }
    }

    if (instancedMap.find(Components::MeshType::INSTANCED_BOX) != instancedMap.end())
    {
        std::vector<glm::vec3> positions;

        for (const auto& e : instancedMap[Components::MeshType::INSTANCED_BOX])
        {
            auto& position = e->GetComponent<Components::Transform>();
            positions.push_back(position.GetPosition());
        }

        RenderObject ro;
        ro.RenderBuffer = m_RenderBufferManager.CreateInstancedBox(positions);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR_INSTANCED);
        ro.Entities = instancedMap[Components::MeshType::INSTANCED_BOX];
        m_RenderQueue.push_back(ro);
    }

    if (instancedMap.find(Components::MeshType::LINE) != instancedMap.end())
    {
        RenderObject ro;
        ro.RenderBuffer = m_RenderBufferManager.CreateInstancedLines(instancedMap[Components::MeshType::LINE]);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
        ro.Entities = instancedMap[Components::MeshType::LINE];
        m_RenderQueue.push_back(ro);
    }

    std::sort(m_RenderQueue.begin(), m_RenderQueue.end(), [](RenderObject a, RenderObject b)
    {
        if (a.Shader == b.Shader)
        {
            return a.RenderBuffer < b.RenderBuffer;
        }
        else
        {
            return a.Shader < b.Shader;
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
    // glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    // glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    Shader* currentShader = nullptr;
    Components::Material* currentMaterial = nullptr;
    RenderBuffer* currentRenderBuffer = nullptr;

    for (RenderObject& ro : m_RenderQueue)
    {
        Shader* shader = ro.Shader;
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
            if (ro.RenderBuffer->IBO != nullptr)
            {
                ro.RenderBuffer->IBO->Bind();
            }

            currentRenderBuffer = renderBuffer;
        }

        shader->SetPerRenderObject(ro.Entities);

        if (m_IsDrawWireFrame)
        {
#ifdef BUILD_DESKTOP
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
        }

        shader->Draw(renderBuffer);

        if (m_IsDrawWireFrame)
        {
#ifdef BUILD_DESKTOP
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
        }
    }
}
