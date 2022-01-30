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
    Material(MaterialType type, Shader* shader, const glm::vec4& color);
    ~Material();

    MaterialType GetType() { return m_Type; }
    Shader* GetShader() { return m_Shader; }
    glm::vec4& GetColor()  { return m_Color; }

    static Material* CreateSolidColorMaterial(const glm::vec4& color) {
        return new Material(MaterialType::SolidColor, Shader::SOLID_COLOR_SHADER, color);
    }
private:
    MaterialType m_Type;
    Shader* m_Shader;
    glm::vec4 m_Color;
};