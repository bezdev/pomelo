#include "shader.h"

Shader* Shader::SOLID_COLOR_SHADER = nullptr;

Shader::Shader(GLuint program, std::vector<ShaderVariable>& variables):
    m_Program(program),
    m_Variables(variables.size())
{
    for (int i = 0; i < variables.size(); i++)
    {
        if (variables[i].Type == ShaderVariableType::ATTRIBUTE) m_Variables[i] = glGetAttribLocation(m_Program, variables[i].Name);
        else if (variables[i].Type == ShaderVariableType::UNIFORM) m_Variables[i] = glGetUniformLocation(m_Program, variables[i].Name);
    }
}