#pragma once

#ifdef BUILD_ANDROID
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#endif

#ifdef BUILD_DESKTOP
#include <glad/glad.h>
#endif

#include <memory>

#include "util.h"

class VertexBuffer {
public:
    VertexBuffer(GLfloat* data, int size, int dataSize, int stride, int index = 0);
    ~VertexBuffer();

    void Bind();
    void Unbind();

    // int GetStride() { return m_Stride; }
    // int GetCount() { return m_Count; }
    // int GetPositionsOffset() { return 0; }

    // bool HasColors() { return mColorsOffset > 0; }
    // int GetColorsOffset() { return mColorsOffset; }
    // void SetColorsOffset(int offset) { mColorsOffset = offset; }

    // bool HasTexCoords() { return mTexCoordsOffset > 0; }
    // void SetTexCoordsOffset(int offset) { mTexCoordsOffset = offset; }
    // int GetTexCoordsOffset() { return mTexCoordsOffset; }

    // GLenum GetPrimitive() { return m_Primitive; }
    // void SetPrimitive(GLenum primitive) { m_Primitive = primitive; }

private:
    GLuint m_VBO;
    int m_Stride;
    int m_Count;
};

class IndexBuffer
{
public:
    IndexBuffer(GLushort *data, int size);
    ~IndexBuffer();

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
