#include "buffers.h"

#include "render/Renderer.h"
#include "engine/Mesh.h"

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

RenderBuffer *RenderBufferManager::CreateFromOBJ(const char* filename)
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

RenderBuffer *RenderBufferManager::CreatePlane()
{
    Mesh::Plane p(10.f, 10.f, 10, 10);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, p.Vertices.data(), (int)p.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(p.Indices.data(), p.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer *RenderBufferManager::CreatePlaneMap()
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

// TODO: move this to font class
struct GlyphMetrics {
    float advance;
    float bitmapLeft;
    float bitmapTop;
    float width;
    float height;
    float u1, v1, u2, v2;
};

// TODO: remove glyph logic from here
RenderBuffer *RenderBufferManager::CreatePlaneTexture()
{
    Mesh::Plane p(10.f, 10.f, 2, 2);

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, p.Vertices.data(), (int)p.Vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, p.TexCoords.data(), (int)p.TexCoords.size() * 2, sizeof(float), 2, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(p.Indices.data(), p.Indices.size() * sizeof(unsigned short));

    return rb;
}

RenderBuffer *RenderBufferManager::CreateText(Text* text)
{
    float advance = 10.f;

    std::vector<VEC3> vertices;
    std::vector<VEC2> texCoords;
    std::vector<unsigned short> indices;

    int i = 0;
    for (char* p = text->GetText(); *p != '\0'; ++p) {
        char c = *p;
        float x0 = 0.f + i * advance;
        float y0 = 0.f;
        float x1 = 10.f + i * advance;
        float y1 = 10.f;

        Glyph gm = text->GetFont()->GetGlyph(c);

        gm.u1 /= text->GetTexture()->GetWidth();
        gm.u2 /= text->GetTexture()->GetWidth();
        gm.v1 /= text->GetTexture()->GetHeight();
        gm.v2 /= text->GetTexture()->GetHeight();

        gm.v1 = 1.f - gm.v1;
        gm.v2 = 1.f - gm.v2;

        vertices.push_back(VEC3(x0, y0, 0.0f));
        vertices.push_back(VEC3(x1, y0, 0.0f));
        vertices.push_back(VEC3(x0, y1, 0.0f));
        vertices.push_back(VEC3(x1, y1, 0.0f));

        texCoords.push_back(VEC2(gm.u1, gm.v1));
        texCoords.push_back(VEC2(gm.u2, gm.v1));
        texCoords.push_back(VEC2(gm.u1, gm.v2));
        texCoords.push_back(VEC2(gm.u2, gm.v2));

        indices.push_back(0 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(2 + i * 4);
        indices.push_back(1 + i * 4);
        indices.push_back(3 + i * 4);

        i++;
    }

    RenderBuffer* rb = CreateRenderBuffer({
        VertexBufferData { 0, vertices.data(), (int)vertices.size() * 3, sizeof(float), 3, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
        VertexBufferData { 1, texCoords.data(), (int)texCoords.size() * 2, sizeof(float), 2, 0, GL_FLOAT, GL_STATIC_DRAW, 0 },
    });

    rb->IBO = new IndexBuffer(indices.data(), indices.size() * sizeof(unsigned short));

    return rb;
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