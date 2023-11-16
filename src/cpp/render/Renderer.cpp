#include "renderer.h"

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
    RenderBufferManager::DestroyInstance();
    TextureManager::DestroyInstance();
    FontManager::DestroyInstance();
    TextManager::DestroyInstance();
}

void Renderer::LoadEntities(const std::vector<Entity>& entities)
{
    ScopeTimer s("LoadEntities");

    RenderBufferManager* renderBufferManager = RenderBufferManager::GetInstance();
    renderBufferManager->Cleanup();
    m_ShaderManager.Cleanup();
    m_RenderQueue.clear();

    std::map<Components::MeshType, std::vector<const Entity*>> instancedMap;

    for (const Entity& entity : entities)
    {
        Components::Material* material = nullptr;
        Components::Mesh* mesh = nullptr;
        Components::Text* text = nullptr;

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

        if (entity.HasComponent<Components::Text>())
        {
            text = &entity.GetComponent<Components::Text>();
        }

        if (material != nullptr && mesh != nullptr)
        {
            if (material->Type == Components::MaterialType::SOLID_COLOR)
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(mesh->Type);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR);
                ro.Entities.push_back(const_cast<Entity*>(&entity));
                m_RenderQueue.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::PIXEL_COLOR && mesh->Type == Components::MeshType::AXIS)
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(Components::MeshType::AXIS);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
                ro.Entities.push_back(const_cast<Entity*>(&entity));
                m_RenderQueue.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::TEXTURE) // TODO: add meshtype check
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(Components::MeshType::PLANE_TEXTURE);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::TEXTURE);
                ro.Texture = TextureManager::GetInstance()->CreateTexture(material->Name);
                ro.Entities.push_back(const_cast<Entity*>(&entity));
                m_RenderQueue.push_back(ro);
            }
        }

        if (text != nullptr)
        {
            Text* t = TextManager::GetInstance()->GetText(text->ID);
            RenderObject ro;
            ro.RenderBuffer = t->GetRenderBuffer();
            ro.Shader = m_ShaderManager.GetShader(ShaderType::FONT);
            ro.Texture = t->GetTexture();
            ro.Entities.push_back(const_cast<Entity*>(&entity));
            m_RenderQueue.push_back(ro);
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
        ro.RenderBuffer = renderBufferManager->CreateInstancedBox(positions);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR_INSTANCED);
        ro.Entities = instancedMap[Components::MeshType::INSTANCED_BOX];
        m_RenderQueue.push_back(ro);
    }

    if (instancedMap.find(Components::MeshType::LINE) != instancedMap.end())
    {
        RenderObject ro;
        ro.RenderBuffer = renderBufferManager->CreateInstancedLines(instancedMap[Components::MeshType::LINE]);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
        ro.Entities = instancedMap[Components::MeshType::LINE];
        m_RenderQueue.push_back(ro);
    }

    // TODO: also sort by material type and maybe texture id
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
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glFrontFace(GL_CW);
    // glCullFace(GL_BACK);

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

            glm::mat4 viewMatrix = shader->GetType() == ShaderType::FONT ? glm::mat4(1.f) : Camera::GetInstance()->GetViewMatrix();
            glm::mat4 projectionMatrix = shader->GetType() == ShaderType::FONT ? glm::ortho(0.0f, static_cast<float>(m_ScreenWidth), 0.0f, static_cast<float>(m_ScreenHeight)) : Camera::GetInstance()->GetProjectionMatrix();
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

        shader->SetPerRenderObject(&ro);

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
