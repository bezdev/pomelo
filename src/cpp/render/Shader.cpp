#include "shader.h"

Shader::Shader():
    m_Variables(10),
    m_Shaders(2)
{}

void Shader::LoadShader(const std::vector<GLuint>& shaders, const std::vector<ShaderVariable>& variables)
{
    m_Program = LinkShader(shaders);
    for (std::size_t i = 0; i < variables.size(); i++)
    {
        if (variables[i].Type == ShaderVariableType::ATTRIBUTE) m_Variables[i] = glGetAttribLocation(m_Program, variables[i].Name);
        else if (variables[i].Type == ShaderVariableType::UNIFORM) m_Variables[i] = glGetUniformLocation(m_Program, variables[i].Name);
    }
}

Shader::~Shader()
{
    for (auto s : m_Shaders) {
        glDeleteShader(s);
    }
    glDeleteProgram(m_Program);
}

SolidColorShader::SolidColorShader():
    Shader()
{
    LoadShader(
        {
            Shader::CompileShader(Util::ReadFile("shaders/SolidColor.vs"), GL_VERTEX_SHADER),
            Shader::CompileShader(Util::ReadFile("shaders/SolidColor.fs"), GL_FRAGMENT_SHADER)
        },
        {
            { ShaderVariableType::ATTRIBUTE, "vPosition" },
            { ShaderVariableType::UNIFORM, "vColor" },
            { ShaderVariableType::UNIFORM, "modelMatrix" },
            { ShaderVariableType::UNIFORM, "viewMatrix" },
            { ShaderVariableType::UNIFORM, "projectionMatrix" },
        }
    );
}

void SolidColorShader::SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void SolidColorShader::SetPerRenderObject(const std::vector<const Entity*>& entities)
{
    auto transform = entities.back()->GetComponent<Components::Transform>();
    auto material = entities.back()->GetComponent<Components::Material>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.GetMM()));
    glUniform4f(m_Variables[1], material.Color.r, material.Color.g, material.Color.b, material.Color.a);
}

void SolidColorShader::Draw(const RenderBuffer* renderBuffer)
{
    glDrawElements(GL_TRIANGLES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
}

SolidColorShaderInstanced::SolidColorShaderInstanced():
    Shader()
{
    LoadShader(
        {
            Shader::CompileShader(Util::ReadFile("shaders/SolidColorInstanced.vs"), GL_VERTEX_SHADER),
            Shader::CompileShader(Util::ReadFile("shaders/SolidColorInstanced.ps"), GL_FRAGMENT_SHADER)
        },
        {
            { ShaderVariableType::ATTRIBUTE, "vPosition" },
            { ShaderVariableType::ATTRIBUTE, "vInstancePosition" },
            { ShaderVariableType::UNIFORM, "vColor" },
            { ShaderVariableType::UNIFORM, "modelMatrix" },
            { ShaderVariableType::UNIFORM, "viewMatrix" },
            { ShaderVariableType::UNIFORM, "projectionMatrix" },
        }
    );
}

void SolidColorShaderInstanced::SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[5], 1, GL_FALSE, projectionMatrix);
}

void SolidColorShaderInstanced::SetPerRenderObject(const std::vector<const Entity*>& entities)
{
    // TODO: fix this hack
    auto transform = entities.back()->GetComponent<Components::Transform>();
    transform.SetPosition(glm::vec3(0,0,0));
    auto material = entities.back()->GetComponent<Components::Material>();
    glUniform4f(m_Variables[2], material.Color.r, material.Color.g, material.Color.b, material.Color.a);
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, glm::value_ptr(transform.GetMM()));
}

void SolidColorShaderInstanced::Draw(const RenderBuffer* renderBuffer)
{
    // TODO: fix this hack
    int instanceCount = renderBuffer->VAO->GetVertexBuffers()[1]->GetCount() / 3;
    glDrawElementsInstanced(GL_TRIANGLES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0, instanceCount);
}

PixelColorShader::PixelColorShader():
    Shader()
{
    LoadShader(
        {
            Shader::CompileShader(Util::ReadFile("shaders/PixelColor.vs"), GL_VERTEX_SHADER),
            Shader::CompileShader(Util::ReadFile("shaders/PixelColor.ps"), GL_FRAGMENT_SHADER)
        },
        {
            { ShaderVariableType::ATTRIBUTE, "position" },
            { ShaderVariableType::ATTRIBUTE, "iColor" },
            { ShaderVariableType::UNIFORM, "modelMatrix" },
            { ShaderVariableType::UNIFORM, "viewMatrix" },
            { ShaderVariableType::UNIFORM, "projectionMatrix" },
        }
    );
}

void PixelColorShader::SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void PixelColorShader::SetPerRenderObject(const std::vector<const Entity*>& entities)
{
    auto transform = entities.back()->GetComponent<Components::Transform>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.GetMM()));
    glLineWidth(3);
}

void PixelColorShader::Draw(const RenderBuffer* renderBuffer)
{
    if (renderBuffer->IBO == nullptr)
    {
        glDrawArrays(GL_LINES, 0, renderBuffer->VAO->GetVertexBuffers()[0]->GetCount());
    }
    else
    {
        glDrawElements(GL_LINES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
    }
}

TextureShader::TextureShader():
    Shader()
{
        LoadShader(
        {
            // TODO: change this back to Texture shader
            Shader::CompileShader(Util::ReadFile("shaders/Font.vs"), GL_VERTEX_SHADER),
            Shader::CompileShader(Util::ReadFile("shaders/Font.fs"), GL_FRAGMENT_SHADER)
        },
        {
            { ShaderVariableType::ATTRIBUTE, "aPosition" },
            { ShaderVariableType::ATTRIBUTE, "aTexCoord" },
            { ShaderVariableType::UNIFORM, "modelMatrix" },
            { ShaderVariableType::UNIFORM, "viewMatrix" },
            { ShaderVariableType::UNIFORM, "projectionMatrix" },
            { ShaderVariableType::UNIFORM, "textureSampler" },
        }
    );
}

void TextureShader::SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void TextureShader::SetPerRenderObject(const std::vector<const Entity*>& entities)
{
    auto transform = entities.back()->GetComponent<Components::Transform>();
    auto material = entities.back()->GetComponent<Components::Material>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.GetMM()));

    // TODO: add to a per material method
    glUniform1i(m_Variables[5], 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureManager::GetInstance()->GetTextureID(entities.back()->GetID()));
}

void TextureShader::Draw(const RenderBuffer* renderBuffer)
{
    glDrawElements(GL_TRIANGLES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
}

FontShader::FontShader():
    Shader()
{
        LoadShader(
        {
            Shader::CompileShader(Util::ReadFile("shaders/Font.vs"), GL_VERTEX_SHADER),
            Shader::CompileShader(Util::ReadFile("shaders/Font.fs"), GL_FRAGMENT_SHADER)
        },
        {
            { ShaderVariableType::ATTRIBUTE, "aPosition" },
            { ShaderVariableType::ATTRIBUTE, "aTexCoord" },
            { ShaderVariableType::UNIFORM, "modelMatrix" },
            { ShaderVariableType::UNIFORM, "viewMatrix" },
            { ShaderVariableType::UNIFORM, "projectionMatrix" },
            { ShaderVariableType::UNIFORM, "textureSampler" },
        }
    );
}

void FontShader::SetVPMatrix(glm::f32* viewMatrix, glm::f32* projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void FontShader::SetPerRenderObject(const std::vector<const Entity*>& entities)
{
    auto transform = entities.back()->GetComponent<Components::Transform>();
    auto material = entities.back()->GetComponent<Components::Material>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.GetMM()));

    // TODO: add to a per material method
    glUniform1i(m_Variables[5], 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TextureManager::GetInstance()->GetTextureID(entities.back()->GetID()));
}

void FontShader::Draw(const RenderBuffer* renderBuffer)
{
    glDrawElements(GL_TRIANGLES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
}

ShaderManager::ShaderManager():
    m_Shaders(static_cast<size_t>(ShaderType::COUNT))
{
}

ShaderManager::~ShaderManager()
{
    Cleanup();
}

Shader *ShaderManager::GetShader(ShaderType type)
{
    auto index = static_cast<size_t>(type);

    if (m_Shaders[index] == nullptr)
    {
        m_Shaders[index] = CreateShader(type);
    }
    return m_Shaders[static_cast<size_t>(type)];
}

void ShaderManager::Cleanup()
{
    for (auto s : m_Shaders)
    {
        delete s;
    }
}
