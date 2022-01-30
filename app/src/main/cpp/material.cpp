#include "material.h"

Material::Material(MaterialType type, Shader* shader, const glm::vec4& color):
    m_Type(type),
    m_Shader(shader),
    m_Color(color)
{
}
