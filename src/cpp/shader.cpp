#include "shader.h"

Shader* Shader::SOLID_COLOR_SHADER = nullptr;
Shader* Shader::PIXEL_COLOR_SHADER = nullptr;

/*Shader::Shader(GLuint program, std::vector<ShaderVariable> &variables):
    m_Program(program),
    m_Variables(variables.size())
{
    LOGI("Shader::Shader");
    for (std::size_t i = 0; i < variables.size(); i++)
    {
        if (variables[i].Type == ShaderVariableType::ATTRIBUTE) m_Variables[i] = glGetAttribLocation(m_Program, variables[i].Name);
        else if (variables[i].Type == ShaderVariableType::UNIFORM) m_Variables[i] = glGetUniformLocation(m_Program, variables[i].Name);
    }
}*/

Shader::Shader(const std::vector<GLuint>& shaders, const std::vector<ShaderVariable>& variables):
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
    auto position = entity->GetComponent<Components::Motion>();
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), position.Position);
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void SolidColorShader::SetPerMaterial(const Components::Material *material)
{
    auto color = material->Color;
    glUniform4f(m_Variables[1], color.r, color.g, color.b, color.a);//0.2f, 0.709803922f, 0.898039216f, 1.0f);
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
    auto position = entity->GetComponent<Components::Motion>();
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.f), position.Position);
    glUniformMatrix4fv(m_Variables[2], 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void PixelColorShader::SetPerMaterial(const Components::Material *material)
{
}