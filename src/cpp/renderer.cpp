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
        rb->VAO->AddVertexBuffer(new VertexBuffer(&axis.Vertices[0], axis.Vertices.size() * sizeof(GLfloat), 3 * sizeof(GLfloat), GL_TRIANGLES));
        rb->VAO->AddVertexBuffer(new VertexBuffer(&axis.Colors[0], axis.Colors.size() * sizeof(GLfloat), 4 * sizeof(GLfloat), GL_TRIANGLES));
        rb->VAO->GetVertexBuffers()[0]->Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        rb->VAO->GetVertexBuffers()[0]->Unbind();
        rb->VAO->GetVertexBuffers()[1]->Bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);
        rb->VAO->GetVertexBuffers()[1]->Unbind();
        rb->VAO->Unbind();
        rb->IBO = new IndexBuffer(reinterpret_cast<GLushort *>(&axis.Indices[0]), axis.Indices.size() * sizeof(GLushort));
        m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)] = rb;

        m_IsInitialized = true;

        LOGI("glGetString(GL_EXTENSIONS): %s", glGetString(GL_EXTENSIONS));
        // glEnable(GL_DEBUG_OUTPUT);
        // glDebugMessageCallback(DebugCallback, nullptr);
    }

    return 0;
}

void Renderer::LoadShaders() {
    {
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
    {
    std::vector<ShaderVariable> variables
    {
        { ShaderVariableType::ATTRIBUTE, "position" },
        { ShaderVariableType::ATTRIBUTE, "iColor" },
        { ShaderVariableType::UNIFORM, "modelMatrix" },
        { ShaderVariableType::UNIFORM, "viewMatrix" },
        { ShaderVariableType::UNIFORM, "projectionMatrix" },
    };

    Shader::PIXEL_COLOR_SHADER = new Shader(Shader::LinkShader(
        Shader::CompileShader("shaders/PixelColor.vs", Util::ReadFile("shaders/PixelColor.vs"), GL_VERTEX_SHADER),
        Shader::CompileShader("shaders/PixelColor.ps", Util::ReadFile("shaders/PixelColor.ps"), GL_FRAGMENT_SHADER)),
        variables);
    }
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
        else if (material->Type == Components::MaterialType::PixelColor && mesh->Type == Components::MeshType::AXIS)
        {
            RenderObject ro;
            ro.RenderBuffer = m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)];
            ro.Shader = Shader::PIXEL_COLOR_SHADER;
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

    m_ProjectionMatrix = glm::frustum<float>(-ratio, ratio, -1.0f, 1.0f, 3, 500);
    // m_ProjectionMatrix = glm::perspective(glm::radians<float>(45.0f), ratio, 3.f, 500.f);
}

void Renderer::Render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    auto viewMatrix = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    static const Shader* currentShader = nullptr;
    static const Components::Material* currentMaterial = nullptr;
    static const RenderBuffer* currentRenderBuffer = nullptr;
    static Components::Mesh* currentMesh = nullptr;
    static GLuint currentPositionHandle = 0;
    static GLuint currentColorHandle = 0;
    static GLuint currentModelMatrixHandle = 0;
    static GLuint currentViewMatrixHandle = 0;
    static GLuint currentProjectionMatrixHandle = 0;
    for (auto ro : m_RenderQueue) {
        const Shader* shader = ro.Shader;
        const Components::Material* material = ro.Material;
        const RenderBuffer* renderBuffer = ro.RenderBuffer;

        if (shader != currentShader) {
            glUseProgram(shader->GetProgram());

            if (shader == Shader::SOLID_COLOR_SHADER)
            {
                auto variables = shader->GetVariables();
                currentPositionHandle = variables[0];
                currentColorHandle = variables[1];
                currentModelMatrixHandle = variables[2];
                currentViewMatrixHandle = variables[3];
                currentProjectionMatrixHandle = variables[4];
            }
            else if (shader == Shader::PIXEL_COLOR_SHADER)
            {
                auto variables = shader->GetVariables();
                currentPositionHandle = variables[0];
                currentColorHandle = variables[1];
                currentModelMatrixHandle = variables[2];
                currentViewMatrixHandle = variables[3];
                currentProjectionMatrixHandle = variables[4];
            }

            glUniformMatrix4fv(currentViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(viewMatrix));
            glUniformMatrix4fv(currentProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));

            currentShader = shader;
        }

        if (renderBuffer != currentRenderBuffer)
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            if (renderBuffer == m_RenderBuffers[static_cast<int>(Components::MeshType::BOX)])
            {
                ro.RenderBuffer->VAO->Bind();
                ro.RenderBuffer->IBO->Bind();
            }
            else if (renderBuffer == m_RenderBuffers[static_cast<int>(Components::MeshType::AXIS)])
            {
                ro.RenderBuffer->VAO->Bind();
                ro.RenderBuffer->IBO->Bind();
        //                 ro.RenderBuffer->VAO->GetVertexBuffers()[0]->Bind();
        // glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * 4, 0);
        // // ro.RenderBuffer->VAO->GetVertexBuffers()[0]->Unbind();
        // ro.RenderBuffer->VAO->GetVertexBuffers()[1]->Bind();
        // glVertexAttribPointer(1, 4, GL_FLOAT, false, 4 * 4, 0);
        // ro.RenderBuffer->VAO->GetVertexBuffers()[1]->Unbind();
                // for (auto b : ro.RenderBuffer->VAO->GetVertexBuffers())
                // {
                //     b->Bind();
                // }
                // CheckGlError("LOL1");
                // // glBindBuffer(GL_ARRAY_BUFFER, renderBuffer->VAO->GetVertexBuffers()[0]h);
                // // glBindBuffer(GLES20.GL_ELEMENT_ARRAY_BUFFER, currentMesh.getIBO());
                // renderBuffer->IBO->Bind();
                // CheckGlError("LOL2");
                // renderBuffer->VAO->GetVertexBuffers()[0]->Bind();
                // CheckGlError("LOL3");
                // glVertexAttribPointer(currentPositionHandle, 3, GL_FLOAT, false, 3 * 4, 0);
                // CheckGlError("LOL4");
                // renderBuffer->VAO->GetVertexBuffers()[1]->Bind();
                // CheckGlError("LOL5");
                // glVertexAttribPointer(currentColorHandle, 4, GL_FLOAT, false, 4 * 4, 0);
                // CheckGlError("LOL6");
                
                glLineWidth(20);

        // glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
        // glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);
                CheckGlError("LOL7");

                // glEnableVertexAttribArray(currentPositionHandle);
                // glEnableVertexAttribArray(currentColorHandle);
            }
            
            currentRenderBuffer = renderBuffer;
        }



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

        if (shader == Shader::SOLID_COLOR_SHADER) glDrawElements(GL_TRIANGLES, ro.RenderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
        else glDrawElements(GL_LINES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
    }
}
