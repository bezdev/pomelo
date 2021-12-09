#include "mesh.h"

// TODO: manage this lifetime
Mesh* Mesh::PRIMITIVE_BOX_TRIANGLE = nullptr;

Mesh::Mesh(std::vector<float> &vertices, std::vector<short> &indices, PrimitiveType primitiveType, MeshType meshType):
    m_Vertices(vertices),
    m_Indices(indices),
    m_PrimitiveType(primitiveType),
    m_Type(meshType)
{
}
