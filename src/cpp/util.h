#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <iostream>
#include <random>

#include <assert.h>

#include "logger.h"

#ifdef BUILD_TEST
#include "Test.h"
#endif

#define ASSERT(condition) { if (!(condition)) { LOGE("ASSERTION FAILED: %s", #condition); } }

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

class Util
{
public:
    static std::vector<char> ReadFile(const char* filename);
    static std::vector<std::string> StringSplit(std::string string, std::string delimiter);
    static std::vector<std::string> StringSplit(const char* string, const char* delimiter);
};
