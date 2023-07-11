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

struct RenderBuffer
{
    VertexArray* VAO;
    IndexBuffer* IBO;
};

struct RenderObject
{
    RenderBuffer* RenderBuffer;
    Shader* Shader;
    Entity* Entity;
    Components::Material* Material;
    Components::Mesh* Mesh;
    Components::Motion* Motion;
};

class Renderer
{
public:
    static Renderer* GetInstance()
    {
        if (!s_Instance) s_Instance = new Renderer();
        return s_Instance;
    }

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    }

    Renderer();
    ~Renderer();

    int Initialize();
    void LoadShaders();

    bool IsInitialized() { return m_IsInitialized; };
    void LoadEntities(const std::vector<Entity>& entities);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    static RenderBuffer* CreateRenderBuffer(std::vector<float>& vertices, std::vector<short>& indices, GLenum primitive)
    {
        RenderBuffer* rb = new RenderBuffer();
        rb->VAO = new VertexArray();
        VertexBuffer* vb = new VertexBuffer(&vertices[0], vertices.size() * sizeof(GLfloat), 3 * sizeof(GLfloat), GL_TRIANGLES);
        rb->VAO->AddVertexBuffer(vb);
        // TODO: remove reinterpret_cast
        rb->IBO = new IndexBuffer(reinterpret_cast<GLushort *>(&indices[0]), indices.size() * sizeof(GLushort));
        rb->VAO->Bind();
        vb->Bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, 0);
        vb->Unbind();
        rb->VAO->Unbind();

        return rb;
    }

    static Renderer* s_Instance;

    bool m_IsInitialized;

    int m_ScreenWidth;
    int m_ScreenHeight;

    glm::mat4 m_ProjectionMatrix;

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
