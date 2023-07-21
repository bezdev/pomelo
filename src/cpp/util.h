#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <random>
#include <cmath>

#include <assert.h>

#include "logger.h"

#ifdef BUILD_TEST
#include "Test.h"
#endif

#define ASSERT(condition) { if (!(condition)) { LOGE("ASSERTION FAILED: %s", #condition); } }

#define GETSET(type, name) \
    private: \
        type name; \
    public: \
        type Get##name() const { return name; } \
        void Set##name(type value) { name = value; }

namespace Constants
{
    constexpr float PI = 3.14159f;
}


inline void THROW(const char* message)
{
    LOGE("%s", message);
    throw std::runtime_error(message);
}

inline void THROW_IF_FALSE(bool value, const char* message)
{
    if (!value)
    {
        THROW(message);
    }
}

inline void THROW_FILE_NOT_FOUND()
{
    throw std::runtime_error("file not found");
}

inline int RANDOM_INTEGER(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

inline float RANDOM_FLOAT(float min, float max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(static_cast<double>(min), static_cast<double>(max));
    return static_cast<float>(dist(gen));
}

inline float GET_ANGLE_IN_RANGE(float radians, float min, float max)
{
    float range = std::abs(max - min);
    float mod = std::fmod(radians, range);

    if (mod > max) {
        return min + std::abs(mod);
    }

    if (mod < min) {
        return max - std::abs(mod);
    }

    return mod;
}

inline float CAP_ANGLE(float radians, float min, float max)
{
    if (radians < min) {
        return min;
    } else if (radians > max) {
        return max;
    } else {
        return radians;
    }
}

inline float TO_DEGRESS(float radians)
{
    return 180 / Constants::PI * radians;
}

class Util
{
public:
    static std::vector<char> ReadFile(const char* filename);
    static std::vector<std::string> SplitString(std::string string, std::string delimiter);
};
