#pragma once

#include <vector>
#include "buffers.h"

namespace Meshes
{
    enum class PrimitiveType
    {
        Triangle,
        Quad
    };

    struct Box
    {
        Box(float width, float height, float depth)
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

        std::vector<float> Vertices;
        std::vector<short> Indices;
    };

    struct Axis
    {
        Axis(float x, float y, float z)
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

        std::vector<float> Vertices;
        std::vector<float> Colors;
        std::vector<short> Indices;
        short LineWidth;
    };

    struct Sphere
    {
        Sphere(float radius, int stacks, int slices)
        {
            std::vector<glm::vec3> vertices;

            // top
            vertices.push_back(glm::vec3(0, radius, 0));

            // middle
            float phiStep = Constants::PI / stacks;
            float thetaStep = 2 * Constants::PI / slices;
            for (int i = 1; i <= stacks - 1; i++)
            {
                double phi = i * phiStep;
                for (int j = 0; j <= slices; j++)
                {
                    double theta = j * thetaStep;

                    vertices.push_back(glm::vec3(
                        radius * std::sin(phi) * std::cos(theta),
                        radius * std::cos(phi),
                        radius * std::sin(phi) * std::sin(theta)));
                }
            }

            // bottom
            vertices.push_back(glm::vec3(0, -radius, 0));

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
            int southPoleIndex = vertices.size() - 1;
            baseIndex = southPoleIndex - ringVertexCount;
            for (int i = 0; i < slices; i++) {
                Indices.push_back(southPoleIndex);
                Indices.push_back(baseIndex + i);
                Indices.push_back(baseIndex + i + 1);
            }

            float[] verticesArray = new float[vertices.size() * 3];
            Vertices(vertices.size() * 3);
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
            for (Vector3 v : vertices) {
                Vector3 n = v.normalize();
                normalsArray[index++] = n.x;
                normalsArray[index++] = n.y;
                normalsArray[index++] = n.z;
            }
        }

        std::vector<float> Vertices;
        std::vector<float> Normals;
        std::vector<short> Indices;
    }

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
