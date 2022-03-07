#include "mesh.h"

// TODO: manage this lifetime
Mesh* Mesh::PRIMITIVE_BOX_TRIANGLE = nullptr;

Mesh::Mesh(std::vector<float> &vertices, std::vector<short> &indices, PrimitiveType primitiveType, MeshType meshType):
    m_Vertices(vertices),
    m_Indices(indices),
    m_PrimitiveType(primitiveType),
    m_Type(meshType)
{
    // TODO: all this should be somewhere else and better, also state should be managed
    m_VertexArray = new VertexArray();
    VertexBuffer* vertexBuffer = new VertexBuffer(&vertices[0], vertices.size() * sizeof(GLfloat), 3 * sizeof(GLfloat));
    m_VertexArray->AddVertexBuffer(vertexBuffer);
    // TODO: remove reinterpret_cast
    m_IndexBuffer = new IndexBuffer(reinterpret_cast<GLushort *>(&indices[0]), indices.size() * sizeof(GLushort));

    m_VertexArray->Bind();
    vertexBuffer->Bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, 0);
    glDisableVertexAttribArray(0);
    vertexBuffer->Unbind();
    m_VertexArray->Unbind();
}
