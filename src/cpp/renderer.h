#pragma once

#include <map>
#include <vector>

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif
#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif


#include "app.h"
#include "model.h"
#include "renderobjectcollection.h"
#include "ECS.h"
#include "shader.h"
#include "scene.h"
#include "util.h"

// TODO: move to other file
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class RenderBuffer
{
public:
    RenderBuffer();

    //void RenderBuffer::Create(std::vector<float> &vertices, std::vector<short> &indices, PrimitiveType primitiveType, MeshType meshType);

    // void RenderBuffer::AddVertexBuffer(
    //     const std::vector<float>& vertices)
    //     // unsigned int attributeIndex,
    //     // unsigned int numComponents,
    //     // unsigned int offset,
    //     // unsigned int stride
    // {
    //     // glBindVertexArray(m_VAO);
    //     // glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[attributeIndex]);
    //     // glVertexAttribPointer(attributeIndex, numComponents, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    //     // glEnableVertexAttribArray(attributeIndex);
    //     // glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     // glBindVertexArray(0);
    //     glGenVertexArrays(1, &m_VAO);
    //     glBindVertexArray(m_VAO);

    //     GLuint vbo;
    //     glGenBuffers(1, &vbo);
    //     glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    //     m_VBOs.push_back(vbo);

    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);
    // }

    // void RenderBuffer::AddIndexBuffer(const std::vector<unsigned int>& indices)
    // {
    //     glGenBuffers(1, &m_IBO);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    //     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    //     m_IndexCount = static_cast<GLsizei>(indices.size());
    // }
    // void Mesh::Draw() const {
    //     glBindVertexArray(m_VAO);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    //     glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    //     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);
    // }
// private:
//     GLuint m_VAO = 0;
//     GLuint m_IBO = 0;
//     GLsizei m_IndexCount = 0;
//     std::vector<GLuint> m_VBOs;
};

struct RenderObject {
    // RenderObject();
    RenderBuffer* RenderBuffer;
    Shader* Shader;
    Entity* Entity;
    // Material* Material;
};

class Renderer
{
public:
    static Renderer* GetInstance()
    {
        if (!s_Instance) s_Instance = new Renderer();
        return s_Instance;
    };

    static void DestoryInstance()
    {
        delete s_Instance;
        s_Instance = nullptr;
    };

    Renderer();
    ~Renderer();

    int Initialize();
    void LoadShaders();

    bool IsInitialized() { return m_IsInitialized; };
    void LoadEntities(const std::vector<Entity>& entities);
    void AddRenderObject(Model* model);
    void UpdateWindowSize(int width, int height);
    void Render();
private:
    static Renderer* s_Instance;

    bool m_IsInitialized;

    int m_ScreenWidth;
    int m_ScreenHeight;

    glm::mat4 m_ProjectionMatrix;

    std::vector<Model*> m_RenderObjects;
    std::vector<RenderObject> m_RenderQueue;

    static void CheckGlError(const char* op) {
        for (GLint error = glGetError(); error; error = glGetError()) {
            LOGI("after %s() glError (0x%x)\n", op, error);
        }
    }

    void Cleanup();
};
