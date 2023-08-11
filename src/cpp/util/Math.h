#pragma once

#include <random>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define VEC2 glm::vec2
#define VEC3 glm::vec3
#define VEC4 glm::vec4
#define QUAT glm::quat
#define MAT4 glm::mat4

extern VEC3 V_ORIGIN;
extern VEC3 V_UP;
extern QUAT Q_DEFAULT;

namespace Generator
{
    static std::vector<float> RandomAverageHeightMap(int x, int y)
    {
        std::vector<float> heights(x * y);

        std::mt19937 rand;
        unsigned int seed = 69;
        rand.seed(seed);
        std::uniform_real_distribution<float> dist(-1.f, 1.f);

        for (size_t i = 0; i < x * y; i++)
        {
            heights[i] = dist(rand);
        }

        return heights;
    }
}