#include "shader.h"

Shader::Shader():
    m_Variables(10),
    m_Shaders(2)
{
}

Shader::Shader(const std::vector<GLuint> &shaders, const std::vector<ShaderVariable> &variables):
    m_Shaders(shaders),
    m_Variables(variables.size())
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
    Shader(
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
        })
{

}

void SolidColorShader::SetVPMatrix(glm::f32 *viewMatrix, glm::f32 *projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void SolidColorShader::SetPerEntity(const Entity *entity)
{
    auto transform = entity->GetComponent<Components::Transform>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.MM));
}

void SolidColorShader::SetPerMaterial(const Components::Material *material)
{
    auto color = material->Color;
    glUniform4f(m_Variables[1], color.r, color.g, color.b, color.a);
}

void SolidColorShader::Draw(const RenderBuffer* renderBuffer)
{
    glDrawElements(GL_TRIANGLES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
}

PixelColorShader::PixelColorShader():
    Shader(
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
        })
{

}

void PixelColorShader::SetVPMatrix(glm::f32 *viewMatrix, glm::f32 *projectionMatrix)
{
    glUniformMatrix4fv(m_Variables[3], 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(m_Variables[4], 1, GL_FALSE, projectionMatrix);
}

void PixelColorShader::SetPerEntity(const Entity *entity)
{
    auto transform = entity->GetComponent<Components::Transform>();
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(transform.MM));
}

void PixelColorShader::SetPerMaterial(const Components::Material *material)
{
    glLineWidth(3);
}

void PixelColorShader::Draw(const RenderBuffer* renderBuffer)
{
    glDrawElements(GL_LINES, renderBuffer->IBO->GetCount(), GL_UNSIGNED_SHORT, 0);
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
