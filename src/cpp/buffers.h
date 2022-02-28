#pragma once

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "util.h"

class VertexBuffer {
private:
    GLuint mVbo;
    GLenum mPrimitive;
    int mStride;
    int mColorsOffset;
    int mTexCoordsOffset;
    int mCount;

public:
    VertexBuffer(GLfloat *geomData, int dataSize, int stride);
    ~VertexBuffer();

    void BindBuffer();
    void UnbindBuffer();

    int GetStride() { return mStride; }
    int GetCount() { return mCount; }
    int GetPositionsOffset() { return 0; }

    bool HasColors() { return mColorsOffset > 0; }
    int GetColorsOffset() { return mColorsOffset; }
    void SetColorsOffset(int offset) { mColorsOffset = offset; }

    bool HasTexCoords() { return mTexCoordsOffset > 0; }
    void SetTexCoordsOffset(int offset) { mTexCoordsOffset = offset; }
    int GetTexCoordsOffset() { return mTexCoordsOffset; }

    GLenum GetPrimitive() { return mPrimitive; }
    void SetPrimitive(GLenum primitive) { mPrimitive = primitive; }
};

class IndexBuffer
{
    public:
        IndexBuffer(GLushort *data, int size);
        ~IndexBuffer();

        void BindBuffer();
        void UnbindBuffer();
        int GetCount() { return mCount; }

    private:
        GLuint mIbo;
        int mCount;
};
