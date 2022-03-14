#pragma once

#include <stdexcept>
#include <vector>
#include <cstdio>
#include <iostream>
#include <assert.h>

#ifdef BUILD_TEST
#include "Test.h"
#endif

#ifdef BUILD_ANDROID
#include <android/log.h>

#define LIB_NAME "fame-native"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LIB_NAME, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LIB_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LIB_NAME, __VA_ARGS__))
#endif

#ifdef BUILD_DESKTOP
#ifndef BUILD_TEST
#define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; } while (0);
#else
#define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; TestSuite::GetInstance().AddLog(std::string(buffer)); } while (0);
#endif
#define LOGD(...) LOG(__VA_ARGS__)
#define LOGI(...) LOG(__VA_ARGS__)
#define LOGE(...) LOG(__VA_ARGS__)
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
