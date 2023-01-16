#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <iostream>
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

class Util
{
public:
    static std::vector<char> ReadFile(const char* filename);
    static std::vector<std::string> StringSplit(std::string string, std::string delimiter);
    static std::vector<std::string> StringSplit(const char* string, const char* delimiter);
};
