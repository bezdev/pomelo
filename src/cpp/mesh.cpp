#include "mesh.h"

Mesh::Box::Box(float width, float height, float depth)
{
    float x2 = 0.5f * width;
    float y2 = 0.5f * height;
    float z2 = 0.5f * depth;

    Vertices = {
        -x2, -y2, -z2,
        -x2, +y2, -z2,
        +x2, +y2, -z2,
        +x2, -y2, -z2,
        -x2, -y2, +z2,
        -x2, +y2, +z2,
        +x2, +y2, +z2,
        +x2, -y2, +z2
    };

    Indices = {
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
}

Mesh::Axis::Axis(float x, float y, float z)
{
    Vertices = {
        0, 0, 0,
        x, 0, 0,
        0, y, 0,
        0, 0, z
    };

    Colors = {
        0.f, 0.f, 0.f, 1.f,
        1.f, 0.f, 0.f, 1.f,
        0.f, 0.f, 1.f, 1.f,
        0.f, 1.f, 0.f, 1.f
    };

    Indices = {
        0, 1,
        0, 2,
        0, 3
    };
    
    LineWidth = 20;
}

Mesh::Sphere::Sphere(float radius, int stacks, int slices)
{
    // top
    Vertices.push_back(0);
    Vertices.push_back(radius);
    Vertices.push_back(0);

    // middle
    float phiStep = Constants::PI / stacks;
    float thetaStep = 2 * Constants::PI / slices;
    for (int i = 1; i <= stacks - 1; i++)
    {
        double phi = i * phiStep;
        for (int j = 0; j <= slices; j++)
        {
            double theta = j * thetaStep;

            Vertices.push_back(radius * std::sin(phi) * std::cos(theta));
            Vertices.push_back(radius * std::cos(phi));
            Vertices.push_back(radius * std::sin(phi) * std::sin(theta));
        }
    }

    // bottom
    Vertices.push_back(0);
    Vertices.push_back(-radius);
    Vertices.push_back(0);

    // top
    for (int i = 1; i <= slices; i++)
    {
        Indices.push_back(0);
        Indices.push_back(i + 1);
        Indices.push_back(i);
    }

    // middle
    int baseIndex = 1;
    int ringVertexCount = slices + 1;
    for (int i = 0; i < stacks - 2; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            Indices.push_back(baseIndex + i * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
            Indices.push_back(baseIndex + i * ringVertexCount + j + 1);
            Indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
        }
    }

    // bot
    int southPoleIndex = Vertices.size() - 1;
    baseIndex = southPoleIndex - ringVertexCount;
    for (int i = 0; i < slices; i++) {
        Indices.push_back(southPoleIndex);
        Indices.push_back(baseIndex + i);
        Indices.push_back(baseIndex + i + 1);
    }

    Normals = std::vector<float>(Vertices.size());
    for (size_t i = 0; i < Vertices.size(); i+=3)
    {
        glm::vec3 n = glm::normalize(glm::vec3(Vertices[i], Vertices[i+1], Vertices[i+2]));
        Normals[i] = n.x;
        Normals[i+1] = n.y;
        Normals[i+2] = n.z;
    }
}
