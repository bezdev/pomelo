#pragma once

#include <vector>
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
    static float GetAverageHeightMapHeight(const std::vector<float>& heightMap, int width, int height, int x, int y)
    {
        float avg = 0.0f;
        float num = 0.0f;

        for(int m = x - 1; m <= x + 1; ++m)
        {
            for(int n = y - 1; n <= y + 1; ++n)
            {
                if (m < 0 || m >= width || n < 0 || n >= height) continue;

                avg += heightMap[m * width + n];
                num += 1.0f;
            }
        }

        return avg / num;
    }

    static std::vector<float> RandomAverageHeightMap(int width, int height)
    {
        std::vector<float> heights(width * height);

        std::mt19937 rand;
        unsigned int seed = 69;
        rand.seed(seed);
        std::uniform_real_distribution<float> dist(-1.f, 1.f);

        for (size_t i = 0; i < width * height; i++)
        {
            heights[i] = dist(rand);
        }

        std::vector<float> averageHeights;
        averageHeights.resize(width * height);
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                averageHeights[i * width + j] = GetAverageHeightMapHeight(heights, width, height, i, j);
            }
        }

        return averageHeights;
    }
}
