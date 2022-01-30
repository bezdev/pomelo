#include "mesh.h"

// TODO: manage this lifetime
Mesh* Mesh::PRIMITIVE_BOX_TRIANGLE = nullptr;

Mesh::Mesh(std::vector<float> &vertices, std::vector<short> &indices, PrimitiveType primitiveType, MeshType meshType):
    m_Vertices(vertices),
    m_Indices(indices),
    m_PrimitiveType(primitiveType),
    m_Type(meshType)
{
    m_VertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(GLfloat), 3 * sizeof(GLfloat));
    // TODO: remove reinterpret_cast
    m_IndexBuffer = new IndexBuffer(reinterpret_cast<GLushort *>(&indices[0]), indices.size() * sizeof(GLushort));
}
