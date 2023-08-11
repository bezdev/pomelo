#pragma once

#include <functional>
#include <list>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <string>

#include "util/Math.h"
#include "util/Util.h"

namespace Mesh
{
    enum class PrimitiveType
    {
        Triangle,
        Quad
    };

    struct OBJDataVertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 Tex;

        bool operator==(const OBJDataVertex& other) const {
            return Position == other.Position && Normal == other.Normal && Tex== other.Tex;
        }
    };

    struct OBJData
    {
        std::vector<glm::vec3> Vertices;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> Tex;
        std::vector<unsigned short> Indices;
    };

    void ParseOBJ(const std::vector<char>& data, OBJData& result);

    struct Box
    {
        Box(float width, float height, float depth);

        std::vector<float> Vertices;
        std::vector<unsigned short> Indices;
    };

    struct Axis
    {
        Axis(float x, float y, float z);

        std::vector<float> Vertices;
        std::vector<float> Colors;
        std::vector<unsigned short> Indices;
        short LineWidth;
    };

    struct Sphere
    {
        Sphere(float radius, int stacks, int slices);

        std::vector<float> Vertices;
        std::vector<float> Normals;
        std::vector<unsigned short> Indices;
    };

    struct Plane
    {
        Plane(float width, float height, int stacks, int slices);
        std::vector<VEC3> Vertices;
        std::vector<VEC3> Normals;
        std::vector<VEC3> Tangents;
        std::vector<VEC2> TexCoords;
        std::vector<unsigned short> Indices;
    };

/*
    struct Geosphere
    {
        Geosphere(float radius, int numSubdivisions)
        {
            numSubdivisions = std::min(numSubdivisions, 5);

            // Approximate a sphere by tessellating an icosahedron.
            float X = 0.525731f;
            float Z = 0.850651f;

            Vector3 pos[] = new Vector3[]
            {
                new Vector3(-X, 0.0f, Z),  new Vector3(X, 0.0f, Z),
                new Vector3(-X, 0.0f, -Z), new Vector3(X, 0.0f, -Z),
                new Vector3(0.0f, Z, X),   new Vector3(0.0f, Z, -X),
                new Vector3(0.0f, -Z, X),  new Vector3(0.0f, -Z, -X),
                new Vector3(Z, X, 0.0f),   new Vector3(-Z, X, 0.0f),
                new Vector3(Z, -X, 0.0f),  new Vector3(-Z, -X, 0.0f)
            };

            int[] k = new int[]
            {
                1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
                1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
                3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
                10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
            };

            ArrayList<Vector3> vertices = new ArrayList<Vector3>(12 * numSubdivisions * 3);
            ArrayList<Integer> indices = new ArrayList<Integer>(60 * numSubdivisions * 3);

            for(int i = 0; i < 12; ++i)
                vertices.add(pos[i]);

            for(int i = 0; i < 60; ++i)
                indices.add(k[i]);

            for(int i = 0; i < numSubdivisions; ++i) {
                Subdivide(vertices, indices);
            }

            ArrayList<Vector3> normals = new ArrayList<Vector3>(vertices.size());

            // Project vertices onto sphere and scale.
            for(int i = 0; i < vertices.size(); ++i)
            {
                // Project onto unit sphere.
                Vector3 n = vertices.get(i).normalize();

                // Project onto sphere.
                Vector3 p = n.multiply(radius);

                vertices.set(i, p);
                normals.add(n);
            }

            float[] verticesArray = new float[vertices.size() * 3];
            int index = 0;
            for (Vector3 v : vertices) {
                verticesArray[index++] = v.x;
                verticesArray[index++] = v.y;
                verticesArray[index++] = v.z;
            }

            short[] indicesArray = new short[indices.size()];
            index = 0;
            for (Integer i : indices) {
                indicesArray[index++] = (short)(int)i;
            }

            float[] normalsArray = new float[vertices.size() * 3];
            index = 0;
            for (Vector3 n : normals) {
                normalsArray[index++] = n.x;
                normalsArray[index++] = n.y;
                normalsArray[index++] = n.z;
            }
        }
    };
*/
}

namespace std
{
template<> struct hash<glm::vec3>
{
    size_t operator()(glm::vec3 const& vec) const
    {
        size_t h1 = hash<float>()(vec.x);
        size_t h2 = hash<float>()(vec.y);
        size_t h3 = hash<float>()(vec.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template<> struct hash<glm::vec2>
{
    size_t operator()(glm::vec2 const& vec) const
    {
        size_t h1 = hash<float>()(vec.x);
        size_t h2 = hash<float>()(vec.y);
        return h1 ^ (h2 << 1);
    }
};

template<> struct hash<Mesh::OBJDataVertex>
{
    size_t operator()(Mesh::OBJDataVertex const& data) const
    {
        size_t h1 = hash<glm::vec3>{}(data.Position);
        size_t h2 = hash<glm::vec3>()(data.Normal);
        size_t h3 = hash<glm::vec2>()(data.Tex);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}