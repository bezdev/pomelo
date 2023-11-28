#include "RenderBuffer.h"

#include "render/Renderer.h"
#include "engine/Mesh.h"

RenderBufferManager* RenderBufferManager::s_Instance = nullptr;

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

void VertexBuffer::UpdateBufferData(void *data, int count, int dataSize)
{
    Bind();

    glBufferData(GL_ARRAY_BUFFER, count * dataSize, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * dataSize, data);

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

IndexBuffer::IndexBuffer(unsigned short* data, int size):
    IndexBuffer(data, size, GL_STATIC_DRAW)
{
}

IndexBuffer::IndexBuffer(unsigned short *data, int size, int usage)
{
    // TODO: fix caller to pass in count instead
    m_Count = size / sizeof(unsigned short);

    glGenBuffers(1, &m_IBO);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    Unbind();
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_IBO);
    m_IBO = 0;
}

void IndexBuffer::Update(unsigned short *data, int size)
{
    Bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);

    Unbind();
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

RenderBuffer* RenderBufferManager::CreateFromOBJ(const char* filename)
{
    auto f = Util::ReadFile(filename);

    Mesh::OBJData d;
    Mesh::ParseOBJ(f, d);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, d.Vertices.data(), (int)d.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });
    rb->IBO = new IndexBuffer(d.Indices.data(), d.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateBox()
{
    Mesh::Box box(1.f, 1.f, 1.f);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, box.Vertices.data(), (int)box.Vertices.size(), sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });
    rb->IBO = new IndexBuffer(box.Indices.data(), box.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateAxis()
{
    Mesh::Axis axis(1.f, 1.f, 1.f);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, axis.Vertices.data(), (int)axis.Vertices.size(), sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, axis.Colors.data(), (int)axis.Colors.size(), sizeof(float), 4, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });
    rb->IBO = new IndexBuffer(axis.Indices.data(), axis.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreatePlane()
{
    Mesh::Plane p(10.f, 10.f, 10, 10);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, p.Vertices.data(), (int)p.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(p.Indices.data(), p.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreatePlaneMap()
{
    Mesh::Plane p(500.f, 500.f, 100, 100);

    auto heights = Generator::RandomAverageHeightMap(100, 100);

    int i = 0;
    for (float height : heights)
    {
        p.Vertices[i++].z = 10 * height;
    }

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, p.Vertices.data(), (int)p.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(p.Indices.data(), p.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreatePlaneTexture()
{
    Mesh::Plane p(10.f, 10.f, 2, 2);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, p.Vertices.data(), (int)p.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, p.TexCoords.data(), (int)p.TexCoords.size() * 2, sizeof(float), 2, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(p.Indices.data(), p.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer *RenderBufferManager::CreateText(std::vector<VEC3> &vertices, std::vector<VEC2> &texCoords, std::vector<unsigned short> &indices)
{
    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, vertices.data(), (int)vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_DYNAMIC_DRAW, 0 },
        VertexBufferData { 1, texCoords.data(), (int)texCoords.size() * 2, sizeof(float), 2, 0, GL_FLOAT, GL_DYNAMIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(indices.data(), indices.size() * sizeof(unsigned short), GL_DYNAMIC_DRAW);

    return rb;
}

void RenderBufferManager::UpdateText(RenderBuffer *renderBuffer, std::vector<VEC3> &vertices, std::vector<VEC2> &texCoords, std::vector<unsigned short> &indices)
{
    const std::vector<VertexBuffer*>& vbs = renderBuffer->VAO->GetVertexBuffers();
    vbs[0]->UpdateBufferData(reinterpret_cast<void*>(vertices.data()), (int)vertices.size() * 3, sizeof(float));
    vbs[1]->UpdateBufferData(reinterpret_cast<void*>(texCoords.data()), (int)texCoords.size() * 2, sizeof(float));
    renderBuffer->IBO->Update(indices.data(), indices.size() * sizeof(unsigned short));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   CHECK_GL_ERROR("RenderBufferManager::UpdateText");
}

RenderBuffer* RenderBufferManager::CreateSphere()
{
    Mesh::Sphere sphere(.5f, 8.f, 8.f);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, sphere.Vertices.data(), (int)sphere.Vertices.size(), sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });
    rb->IBO = new IndexBuffer(sphere.Indices.data(), sphere.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer* RenderBufferManager::CreateInstancedBox(std::vector<glm::vec3>& positions)
{
    Mesh::Box box(1.f, 1.f, 1.f);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, box.Vertices.data(), (int)box.Vertices.size(), sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, positions.data(), (int)positions.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 1 },
    });
    rb->IBO = new IndexBuffer(box.Indices.data(), box.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer *RenderBufferManager::CreateInstancedLines(const std::vector<const Entity *>& entities)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec4> colors;

    short i = 0;
    for (const Entity* e : entities)
    {
        auto& transform = e->GetComponent<Components::Transform>();
        vertices.push_back(transform.GetPosition());
        vertices.push_back(transform.GetPosition() + transform.GetScale());

        auto& material = e->GetComponent<Components::Material>();
        colors.push_back(material.Color);
        colors.push_back(material.Color);
    }

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, vertices.data(), (int)vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, colors.data(), (int)colors.size() * 4, sizeof(float), 4, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });
 
    return rb;
}
