#include "buffers.h"

VertexBuffer::VertexBuffer(float* data, int size, int dataSize, int stride, int index)
{
    size = size * dataSize;
    stride = stride * dataSize;

    ASSERT(size % stride == 0);

    m_VBO = 0;
    m_Stride = stride;
    m_Count = size / stride;

    glGenBuffers(1, &m_VBO);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(index);
    // LOGD("glVertexAttribPointer %d, %d", index, stride/ dataSize);
    glVertexAttribPointer(index, stride / dataSize, GL_FLOAT, false, 0, 0);
    Unbind();
}

void VertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_VBO);
    m_VBO = 0;
}

IndexBuffer::IndexBuffer(unsigned short* data, int size)
{
    m_Count = size / sizeof(unsigned short);

    glGenBuffers(1, &m_IBO);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    Unbind();
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_IBO);
    m_IBO = 0;
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_VAO);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_VAO);
    m_VAO = 0;
}

void VertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
{
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::Bind()
{
    glBindVertexArray(m_VAO);
    for (std::size_t i = 0; i < m_VertexBuffers.size(); i++)
    {
        glEnableVertexAttribArray(i);
    }
}

void VertexArray::Unbind()
{
    for (std::size_t i = 0; i < m_VertexBuffers.size(); i++)
    {
        glDisableVertexAttribArray(i);
    }
    glBindVertexArray(0);
}
