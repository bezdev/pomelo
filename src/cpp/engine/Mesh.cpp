#include "engine/Mesh.h"

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
            Vertices.push_back(radius * static_cast<float>(std::cos(phi)));
            Vertices.push_back(radius * std::sin(phi) * std::sin(theta));
        }
    }

    // bottom
    Vertices.push_back(0);
    Vertices.push_back(-radius);
    Vertices.push_back(0);

    // top
    for (unsigned short i = 1; i <= slices; i++)
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
    int southPoleIndex = Vertices.size() / 3 - 1;
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

Mesh::Plane::Plane(float width, float height, int stacks, int slices)
{
    unsigned int vertexCount = stacks * slices;
    unsigned int faceCount   = (stacks - 1) * (slices - 1) * 2;

    float halfWidth = 0.5f * width;
    float halfHeight = 0.5f * height;

    float dx = width / (stacks - 1);
    float dy = height / (slices - 1);

    float du = 1.0f / (stacks - 1);
    float dv = 1.0f / (slices - 1);

    Vertices.resize(vertexCount);
    Normals.resize(vertexCount);
    Tangents.resize(vertexCount);
    TexCoords.resize(vertexCount);
    for(int i = 0; i < slices; ++i)
    {
        float y = halfHeight- i*dy;
        for(unsigned int j = 0; j < stacks; ++j)
        {
            float x = -halfWidth + j*dx;

            Vertices[i*stacks+j] = VEC3(x, y, 0.f);
            Normals[i*stacks+j] = VEC3(0.0f, 1.0f, 0.0f);
            Tangents[i*stacks+j] = VEC3(1.0f, 0.0f, 0.0f);
            TexCoords[i*stacks+j] = VEC2(j*du, i*dv);
        }
    }

    Indices.resize(faceCount*3);

    unsigned int k = 0;
    for(int i = 0; i < slices - 1; ++i)
    {
        for(int j = 0; j < stacks - 1; ++j)
        {
            Indices[k]   = i*stacks+j;
            Indices[k+1] = (i+1)*stacks+j;
            Indices[k+2] = i*stacks+j+1;

            Indices[k+3] = (i+1)*stacks+j;
            Indices[k+4] = (i+1)*stacks+j+1;
            Indices[k+5] = i*stacks+j+1;

            k += 6;
        }
    }
}

void Mesh::ParseOBJ(const std::vector<char> &data, OBJData &result)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texs;
    std::unordered_map<OBJDataVertex, unsigned short> vertexToIndex;
    std::unordered_map<int, std::list<int>> duplicateVertices;

    std::stringstream ss(std::string(data.begin(), data.end()));
    std::string line;
    while (std::getline(ss, line))
    {
        std::stringstream lineStream(line);
        std::string type;
        lineStream >> type;

        if (type == "v")
        {
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (type == "vn")
        {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "vt")
        {
            glm::vec2 tex;
            lineStream >> tex.x >> tex.y;
            texs.push_back(tex);
        }
        else if (type == "f")
        {
            for (int i = 0; i < 3; ++i)
            {
                unsigned int pIndex, nIndex, tIndex;
                char slash;
                lineStream >> pIndex >> slash >> tIndex >> slash >> nIndex;

                OBJDataVertex d;
                d.Position = vertices[--pIndex];
                d.Normal = normals[--nIndex];
                d.Tex = texs[--tIndex];

                auto it = vertexToIndex.find(d);
                if (it != vertexToIndex.end()) {
                    // a duplicate vertex exists - reuse its index
                    result.Indices.push_back(it->second);
                } else {
                    // this vertex doesn't exist - add it to vertices and record its index
                    result.Vertices.push_back(d.Position);
                    result.Normals.push_back(d.Normal);
                    result.Tex.push_back(d.Tex);
                    unsigned int index = result.Vertices.size() - 1;
                    result.Indices.push_back(index);
                    vertexToIndex[d] = index;
                }
            }
        }
    }
}