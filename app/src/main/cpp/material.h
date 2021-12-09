#pragma once

#include "shader.h"
#include "glm/glm.hpp"

// TODO: refactor
enum MaterialType
{
    SolidColor
};

class Material {
public:
    Material(MaterialType type, unsigned int shaderID, const glm::vec4& color);
    ~Material();

    MaterialType GetType() { return m_Type; }
    unsigned int GetShaderID() { return m_ShaderID; }

    static Material* CreateSolidColorMaterial(const glm::vec4& color) {
        return new Material(MaterialType::SolidColor, Shader::SOLID_COLOR_SHADER, color);
    }
private:
    MaterialType m_Type;
    unsigned int m_ShaderID;
    glm::vec4 m_Color;
};