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
        Box(float width, float height, float depth):
            PrimitiveType(PrimitiveType::Triangle)
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
        PrimitiveType PrimitiveType;
    };

    struct Axis
    {
        Axis(float x, float y, float z) {
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
}
