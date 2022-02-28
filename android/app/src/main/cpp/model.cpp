#include "model.h"

Model::Model(glm::vec3 position, Mesh* mesh, Material* material):
    m_Position(position),
    m_Mesh(mesh),
    m_Material(material)
{
}
