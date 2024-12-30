#include "render/Renderer.h"
#include "Renderer.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer():
    m_IsInitialized(false),
    m_ScreenWidth(0),
    m_ScreenHeight(0),
    m_IsDrawWireFrame(false),
    m_GUIRenderObjects(10),
    m_FPSTextElement(nullptr)
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
    m_RenderObjects.clear();

    std::map<Components::MeshType, std::vector<ENTITY>> instancedMap;

#ifdef USE_ENTT
    for (auto entity : ENTT::GetInstance()->view<Components::Transform>())
#else
    for (auto& e : ECS::GetInstance()->GetEntities())
#endif
    {
#ifndef USE_ENTT
        auto entity = &e;
#endif
        Components::Material* material = nullptr;
        Components::Mesh* mesh = nullptr;
        Components::Text* text = nullptr;

        if (HAS_COMPONENT(entity, Components::Material))
        {
            material = &GET_COMPONENT(entity, Components::Material);
        }

        if (HAS_COMPONENT(entity, Components::Mesh))
        {
            mesh = &GET_COMPONENT(entity, Components::Mesh);

            if (mesh->Type == Components::MeshType::INSTANCED_BOX || mesh->Type == Components::MeshType::LINE )
            {
                instancedMap[mesh->Type].push_back(entity);
                continue;
            }
        }

        if (HAS_COMPONENT(entity, Components::Text))
        {
            text = &GET_COMPONENT(entity, Components::Text);
        }

        if (material != nullptr && mesh != nullptr)
        {
            if (material->Type == Components::MaterialType::SOLID_COLOR)
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(mesh->Type);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR);
                ro.Entities.push_back(entity);
                m_RenderObjects.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::PIXEL_COLOR && mesh->Type == Components::MeshType::AXIS)
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(Components::MeshType::AXIS);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
                ro.Entities.push_back(entity);
                m_RenderObjects.push_back(ro);
            }
            else if (material->Type == Components::MaterialType::TEXTURE) // TODO: add meshtype check
            {
                RenderObject ro;
                ro.RenderBuffer = renderBufferManager->GetRenderBuffer(Components::MeshType::PLANE_TEXTURE);
                ro.Shader = m_ShaderManager.GetShader(ShaderType::TEXTURE);
                ro.Texture = TextureManager::GetInstance()->CreateTexture(material->Name);
                ro.Entities.push_back(entity);
                m_RenderObjects.push_back(ro);
            }
        }

        if (text != nullptr)
        {
            Text* t = TextManager::GetInstance()->GetText(text->ID);
            RenderObject ro;
            ro.RenderBuffer = t->GetRenderBuffer();
            ro.Shader = m_ShaderManager.GetShader(ShaderType::FONT);
            ro.Texture = t->GetTexture();
            ro.Entities.push_back(entity);
            m_RenderObjects.push_back(ro);
        }
    }

    if (instancedMap.find(Components::MeshType::INSTANCED_BOX) != instancedMap.end())
    {
        std::vector<glm::vec3> positions;

        for (auto e : instancedMap[Components::MeshType::INSTANCED_BOX])
        {
            auto& position = GET_COMPONENT(e, Components::Transform);
            positions.push_back(position.GetPosition());
        }

        RenderObject ro;
        ro.RenderBuffer = renderBufferManager->CreateInstancedBox(positions);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::SOLID_COLOR_INSTANCED);
        ro.Entities = instancedMap[Components::MeshType::INSTANCED_BOX];
        m_RenderObjects.push_back(ro);
    }

    if (instancedMap.find(Components::MeshType::LINE) != instancedMap.end())
    {
        RenderObject ro;
        ro.RenderBuffer = renderBufferManager->CreateInstancedLines(instancedMap[Components::MeshType::LINE]);
        ro.Shader = m_ShaderManager.GetShader(ShaderType::PIXEL_COLOR);
        ro.Entities = instancedMap[Components::MeshType::LINE];
        m_RenderObjects.push_back(ro);
    }

    // TODO: also sort by material type and maybe texture id
    std::sort(m_RenderObjects.begin(), m_RenderObjects.end(), [](RenderObject a, RenderObject b)
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

void Renderer::LoadGUI()
{
    m_FPSTextElement = static_cast<GUI::TextElement*>(m_GUI.AddElement(m_GUI.CreateTextElement(
        VEC2(m_ScreenWidth - 10, m_ScreenHeight - 10),
        std::string("                    "),
        GUI::TextProperties {
            GUI::AnchorType::TOP_RIGHT,
            24,
            V_COLOR_RED
        }
    )));
    AddGUIElement(m_FPSTextElement);
}

void Renderer::AddGUIElement(GUI::Element* element)
{
    GUI::TextElement* textElement = static_cast<GUI::TextElement*>(element);
    if (textElement != nullptr)
    {
        Text* t = TextManager::GetInstance()->CreateText(textElement->GetText());
        textElement->SetTextObject(t);
        m_GUIRenderObjects[textElement->GetID()] = GUIRenderObject {
            t->GetRenderBuffer(),
            m_ShaderManager.GetShader(ShaderType::FONT),
            t->GetTexture(),
            element
        };
    }
}


void Renderer::UpdateWindowSize(int width, int height)
{
    m_ScreenWidth = width;
    m_ScreenHeight = height;

    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

    m_GUI.UpdateViewSize(width, height);
    Camera::GetInstance()->UpdateViewSize(width, height);
}

void Renderer::Render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    // glFrontFace(GL_CW);
    // glCullFace(GL_BACK);

    Shader* currentShader = nullptr;
    Components::Material* currentMaterial = nullptr;
    RenderBuffer* currentRenderBuffer = nullptr;

    for (RenderObject& ro : m_RenderObjects)
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

    RenderGUI();

    CHECK_GL_ERROR("Renderer::Render");
}

void Renderer::RenderGUI()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader* currentShader = nullptr;
    RenderBuffer* currentRenderBuffer = nullptr;

    for (auto element : m_GUI)
    {
        GUIRenderObject& ro = m_GUIRenderObjects[element->GetID()];

        if (ro.Element == nullptr) continue;

        Shader* shader = ro.Shader;
        RenderBuffer* renderBuffer = ro.RenderBuffer;
        Texture* texture = ro.Texture;

        if (shader != currentShader)
        {
            shader->Use();

            glm::mat4 viewMatrix = glm::mat4(1.f);
            glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(m_ScreenWidth), 0.0f, static_cast<float>(m_ScreenHeight));
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

        auto textElement = static_cast<GUI::TextElement*>(ro.Element);
        shader->SetUniformMatrix4fv(shader->GetVariables()[2], glm::value_ptr(textElement->GetTransform()));
        shader->SetUniform1i(shader->GetVariables()[5], 0);
        shader->BindTexture(texture->GetTextureID());
        shader->Draw(currentRenderBuffer);
    }
}

void Renderer::UpdateFPS(const std::string &fps)
{
    if (m_FPSTextElement == nullptr) return;
    Text* t = m_FPSTextElement->GetTextObject();
    t->UpdateText(fps);
}
