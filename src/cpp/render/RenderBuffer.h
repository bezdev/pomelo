#pragma once

#include <memory>
#include <random>

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include "engine/ECS.h"
#include "util/Math.h"
#include "util/Util.h"

struct VertexBufferData
{
    int Index;
    void* Data;
    int Count;
    int DataSize;
    int Stride;
    int Offset;
    int Type;
    int Usage;
    int Divisor;
};

class VertexBuffer
{
public:
    static VertexBuffer* CreateVertexBuffer(const VertexBufferData& data)
    {
        return new VertexBuffer(
            data.Index,
            data.Data,
            data.Count,
            data.DataSize,
            data.Stride,
            data.Offset,
            data.Type,
            data.Usage,
            data.Divisor
        );
    }

    VertexBuffer(int index, void* data, int count, int dataSize, int stride, int offset, int type, int usage, int divisor);
    ~VertexBuffer();

    void UpdateBufferData(void* data, int count, int dataSize);
    void Bind();
    void Unbind();

    int GetCount() { return m_Count; }
private:
    GLuint m_VBO;
    int m_Stride;
    int m_Offset;
    int m_Count;
};

class IndexBuffer
{
public:
    IndexBuffer(unsigned short* data, int size);
    IndexBuffer(unsigned short* data, int size, int usage);
    ~IndexBuffer();

    void Update(unsigned short* data, int size);
    void Bind();
    void Unbind();
    int GetCount() { return m_Count; }

private:
    GLuint m_IBO;
    int m_Count;
};

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddVertexBuffer(VertexBuffer* vertexBuffer);
    const std::vector<VertexBuffer*>& GetVertexBuffers() { return m_VertexBuffers; }
    void Bind();
    void Unbind();

private:
    std::vector<VertexBuffer*> m_VertexBuffers;
    GLuint m_VAO;
};

struct RenderBuffer
{
    VertexArray* VAO;
    IndexBuffer* IBO;
};

#define DEFINE_RENDER_BUFFER_ENUM_CLASS_LIST(MACRO)                     \
    MACRO(BOX, CreateBox)                                               \
    MACRO(AXIS, CreateAxis)                                             \
    MACRO(PLANE, CreatePlane)                                           \
    MACRO(PLANE_MAP, CreatePlaneMap)                                    \
    MACRO(PLANE_TEXTURE, CreatePlaneTexture)                            \
    MACRO(SPHERE, CreateSphere)                                         \
    MACRO(BLENDER_AXIS, CreateBlenderAxis)                              \
    MACRO(BLENDER_MONKEY, CreateBlenderMonkey)                          \

#define GENERATE_CASE_VALUE(name, func) case Components::MeshType::name: return func();

class RenderBufferManager
{
public:
    static RenderBufferManager* GetInstance()
    {
        if (!s_Instance) s_Instance = new RenderBufferManager();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    RenderBufferManager();
    ~RenderBufferManager();

    static RenderBuffer* CreateBox();
    static RenderBuffer* CreateAxis();
    static RenderBuffer* CreatePlane();
    static RenderBuffer* CreateSphere();
    static RenderBuffer* CreateInstancedBox(std::vector<glm::vec3>& positions);
    static RenderBuffer* CreateInstancedLines(const std::vector<ENTITY>& entities);
    static RenderBuffer* CreateBlenderAxis() { return CreateFromOBJ("assets/obj/unit_axis.obj"); }
    static RenderBuffer* CreateBlenderMonkey() { return CreateFromOBJ("assets/obj/monkey.obj"); }
    static RenderBuffer* CreatePlaneMap();
    static RenderBuffer* CreatePlaneTexture();
    static RenderBuffer* CreateText(std::vector<VEC3>& vertices, std::vector<VEC2>& texCoords, std::vector<unsigned short>& indices);
    static void UpdateText(RenderBuffer* renderBuffer,  std::vector<VEC3>& vertices,  std::vector<VEC2>& texCoords, std::vector<unsigned short>& indices);

    static RenderBuffer* CreateFromOBJ(const char* filename);

    static RenderBuffer* CreateRenderBuffer(const std::vector<VertexBufferData>& bufferData)
    {
        RenderBuffer* rb = new RenderBuffer();
        rb->VAO = new VertexArray();
        rb->VAO->Bind();
        for (auto vbd : bufferData)
        {
            rb->VAO->AddVertexBuffer(VertexBuffer::CreateVertexBuffer(vbd));
        }
        rb->VAO->Unbind();

        return rb;
    }

    RenderBuffer* CreateRenderBuffer(Components::MeshType type)
    {
        switch (type)
        {
            DEFINE_RENDER_BUFFER_ENUM_CLASS_LIST(GENERATE_CASE_VALUE)
        }

        return nullptr;
    }

    RenderBuffer* GetRenderBuffer(Components::MeshType type)
    {
        auto index = static_cast<size_t>(type);

        if (m_RenderBuffers[index] == nullptr)
        {
            m_RenderBuffers[index] = CreateRenderBuffer(type);
        }
        return m_RenderBuffers[static_cast<size_t>(type)];
    }

    // TODO:
    RenderBuffer* GetRenderBuffer(const Components::Mesh& mesh)
    {
        return nullptr;
    }

    void Cleanup()
    {
        for (auto rb : m_RenderBuffers)
        {
            delete rb;
        }
    }

private:
    static RenderBufferManager* s_Instance;
    std::vector<RenderBuffer*> m_RenderBuffers;
};

#undef GENERATE_CASE_VALUE
#undef DEFINE_RENDER_BUFFER_ENUM_CLASS_LIST
