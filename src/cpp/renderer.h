#pragma once

#include <map>
#include <vector>
#include <unordered_map>

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif


#include "app.h"
#include "ECS.h"
#include "shader.h"
#include "mesh.h"
#include "buffers.h"
#include "scene.h"
#include "util.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct RenderObject
{
    ::RenderBuffer* RenderBuffer;
    ::Shader* Shader;
    ::Entity* Entity;
    Components::Material* Material;
    Components::Mesh* Mesh;
    Components::Transform* Transform;
};

#define DEFINE_RENDER_BUFFER_ENUM_CLASS_LIST(MACRO) \
    MACRO(BOX, CreateBox) \
    MACRO(AXIS, CreateAxis)

class RenderBufferManager
{
public:
    RenderBufferManager();
    ~RenderBufferManager();

#define GENERATE_CASE_VALUE(name, func) case Components::MeshType::name: return func();

    static RenderBuffer* CreateBox()
    {
        Meshes::Box b(1.f, 1.f, 1.f);
        RenderBuffer* rb = new RenderBuffer();
        rb->VAO = new VertexArray();
        rb->VAO->Bind();
        VertexBuffer* vb = new VertexBuffer(b.Vertices.data(), b.Vertices.size(), sizeof(float), 3, 0);
        rb->VAO->AddVertexBuffer(vb);
        rb->VAO->Unbind();

        rb->IBO = new IndexBuffer(b.Indices.data(), b.Indices.size() * sizeof(unsigned short));

        return rb;
    }

    static RenderBuffer* CreateAxis()
    {
        Meshes::Axis axis(1.f, 1.f, 1.f);
        RenderBuffer *rb = new RenderBuffer();
        rb->VAO = new VertexArray();
        rb->VAO->Bind();
        rb->VAO->AddVertexBuffer(new VertexBuffer(axis.Vertices.data(), axis.Vertices.size(), sizeof(float), 3, 0));
        rb->VAO->AddVertexBuffer(new VertexBuffer(axis.Colors.data(), axis.Colors.size(), sizeof(float), 4, 1));
        rb->VAO->Unbind();
    
        rb->IBO = new IndexBuffer(axis.Indices.data(), axis.Indices.size() * sizeof(unsigned short));

        return rb;
    }

    RenderBuffer* CreateRenderBuffer(Components::MeshType type)
    {
        switch(type)
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

    void Cleanup()
    {
        for (auto rb : m_RenderBuffers)
        {
            delete rb;
        }
    }

private:
    std::vector<RenderBuffer*> m_RenderBuffers;
};

#undef GENERATE_CASE_VALUE
#undef DEFINE_RENDER_BUFFER_ENUM_CLASS_LIST

class Renderer
{
public:
    static Renderer* GetInstance()
    {
        if (!s_Instance) s_Instance = new Renderer();
        return s_Instance;
    }

    static void DestroyInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    Renderer();
    ~Renderer();

    int Initialize();

    bool IsInitialized() { return m_IsInitialized; };
    void LoadEntities(const std::vector<Entity>& entities);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    static Renderer* s_Instance;

    bool m_IsInitialized;
    bool m_IsDrawWireFrame;

    int m_ScreenWidth;
    int m_ScreenHeight;

    ShaderManager m_ShaderManager;
    RenderBufferManager m_RenderBufferManager;

    std::vector<RenderObject> m_RenderQueue;
    std::unordered_map<int, RenderBuffer*> m_RenderBuffers;

    static void CheckGLError(const char* op)
    {
        for (GLint error = glGetError(); error; error = glGetError())
        {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    void Cleanup();
};
