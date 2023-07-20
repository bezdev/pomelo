#include "buffers.h"

#include "renderer.h"
#include "mesh.h"

VertexBuffer::VertexBuffer(int index, void* data, int count, int dataSize, int stride, int offset, int type, int usage, int divisor)
{
    int size = count * dataSize;
    int strideSize = stride * dataSize;

    ASSERT(size % strideSize == 0);

    m_VBO = 0;
    m_Stride = stride;
    m_Count = count;
    m_Offset = offset;

    glGenBuffers(1, &m_VBO);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, stride, type, false, strideSize, reinterpret_cast<void*>(offset));
    CHECK_GL_ERROR("VertexBuffer::VertexBuffer");
    if (divisor > 0)
    {
        glVertexAttribDivisor(index, divisor);
    }

    CHECK_GL_ERROR("VertexBuffer::VertexBuffer");

    Unbind();
}

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

RenderBufferManager::RenderBufferManager():
    m_RenderBuffers(static_cast<size_t>(Components::MeshType::COUNT))
{
}

RenderBufferManager::~RenderBufferManager()
{
    Cleanup();
}

RenderBuffer* RenderBufferManager::CreateBox()
{
    Mesh::Box box(1.f, 1.f, 1.f);
    RenderBuffer* rb = new RenderBuffer();
    rb->VAO = new VertexArray();
    rb->VAO->Bind();
    VertexBuffer* vb = new VertexBuffer(box.Vertices.data(), box.Vertices.size(), sizeof(float), 3, 0);
    rb->VAO->AddVertexBuffer(vb);
    rb->VAO->Unbind();

    rb->IBO = new IndexBuffer(box.Indices.data(), box.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateAxis()
{
    Mesh::Axis axis(1.f, 1.f, 1.f);
    RenderBuffer *rb = new RenderBuffer();
    rb->VAO = new VertexArray();
    rb->VAO->Bind();
    rb->VAO->AddVertexBuffer(new VertexBuffer(axis.Vertices.data(), axis.Vertices.size(), sizeof(float), 3, 0));
    rb->VAO->AddVertexBuffer(new VertexBuffer(axis.Colors.data(), axis.Colors.size(), sizeof(float), 4, 1));
    rb->VAO->Unbind();

    rb->IBO = new IndexBuffer(axis.Indices.data(), axis.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateSphere()
{
    Mesh::Sphere sphere(.5f, 8.f, 8.f);

    RenderBuffer *rb = new RenderBuffer();
    rb->VAO = new VertexArray();
    rb->VAO->Bind();
    rb->VAO->AddVertexBuffer(new VertexBuffer(sphere.Vertices.data(), sphere.Vertices.size(), sizeof(float), 3, 0));
    rb->VAO->Unbind();

    rb->IBO = new IndexBuffer(sphere.Indices.data(), sphere.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateInstancedBox(std::vector<glm::vec3>& positions)
{
    Mesh::Box box(1.f, 1.f, 1.f);

    RenderBuffer* rb = new RenderBuffer();
    rb->VAO = new VertexArray();
    rb->VAO->Bind();
    VertexBuffer* vb = new VertexBuffer(box.Vertices.data(), box.Vertices.size(), sizeof(float), 3, 0);
    rb->VAO->AddVertexBuffer(vb);
    const VertexBufferData<float> vbd = {
        1,
        reinterpret_cast<float*>(positions.data()),
        positions.size() * 3,
        3,
        0,
        GL_FLOAT,
        GL_STATIC_DRAW,
        1
    };
    rb->VAO->AddVertexBuffer(VertexBuffer::CreateVertexBuffer(vbd));
    rb->VAO->Unbind();

    rb->IBO = new IndexBuffer(box.Indices.data(), box.Indices.size() * sizeof(unsigned short));

    return rb;
}
