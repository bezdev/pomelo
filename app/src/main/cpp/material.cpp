#include "material.h"

Material::Material(MaterialType type, unsigned int shaderID, const glm::vec4& color):
    m_Type(type),
    m_ShaderID(shaderID),
    m_Color(color)
{
}
