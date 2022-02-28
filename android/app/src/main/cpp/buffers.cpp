#include "buffers.h"

VertexBuffer::VertexBuffer(GLfloat *geomData, int size, int stride) {
    ASSERT(size % stride == 0);

    mPrimitive = GL_TRIANGLES;
    mVbo = 0;
    mStride = stride;
    mColorsOffset = mTexCoordsOffset = 0;
    mCount = size / stride;

    // build VBO
    glGenBuffers(1, &mVbo);
    BindBuffer();
    glBufferData(GL_ARRAY_BUFFER, size, geomData, GL_STATIC_DRAW);
    UnbindBuffer();
}

void VertexBuffer::BindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
}

void VertexBuffer::UnbindBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &mVbo);
    mVbo = 0;
}

IndexBuffer::IndexBuffer(GLushort *data, int size) {
    mCount = size / sizeof(GLushort);

    glGenBuffers(1, &mIbo);
    BindBuffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    UnbindBuffer();
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &mIbo);
    mIbo = 0;
}

void IndexBuffer::BindBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIbo);
}

void IndexBuffer::UnbindBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
