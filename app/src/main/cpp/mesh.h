#pragma once

#include <vector>

enum class PrimitiveType {
    Triangle,
    Quad
};

// TODO: refactor
enum MeshType
{
    PRIMITIVE_BOX,
    PRIMITIVE_AXIS
};

class Mesh {
public:
    static Mesh* PRIMITIVE_BOX_TRIANGLE;

    Mesh(std::vector<float>& vertices, std::vector<short>& indices, PrimitiveType primitiveType, MeshType meshType);

    MeshType GetType() const { return m_Type; }
    PrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }

    static void CreateBoxMesh(float width, float height, float depth) {
        float x2 = 0.5f * width;
        float y2 = 0.5f * height;
        float z2 = 0.5f * depth;

        std::vector<float> vertices {
            -x2, -y2, -z2,
            -x2, +y2, -z2,
            +x2, +y2, -z2,
            +x2, -y2, -z2,
            -x2, -y2, +z2,
            -x2, +y2, +z2,
            +x2, +y2, +z2,
            +x2, -y2, +z2
        };

        std::vector<short> indices {
            // front face
            0, 1, 2,
            0, 2, 3,
            // back face
            4, 6, 5,
            4, 7, 6,
            // left face
            4, 5, 1,
            4, 1, 0,
            // right face
            3, 2, 6,
            3, 6, 7,
            // top face
            1, 5, 6,
            1, 6, 2,
            // bottom face
            4, 0, 3,
            4, 3, 7
        };

        PRIMITIVE_BOX_TRIANGLE = new Mesh(vertices, indices, PrimitiveType::Triangle, MeshType::PRIMITIVE_BOX);
    }

private:
    std::vector<float> m_Vertices;
    std::vector<short> m_Indices;
    PrimitiveType m_PrimitiveType;
    MeshType m_Type;
};
