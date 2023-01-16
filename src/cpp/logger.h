#pragma once

#include <iostream>

#ifdef BUILD_ANDROID
#include <android/log.h>

#define LIB_NAME "pomelo-native"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LIB_NAME, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LIB_NAME, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LIB_NAME, __VA_ARGS__))
#endif

class Logger {
public:
    enum class Level {
        Verbose,
        Debug,
        Info,
        Error
    };

    static Logger* GetInstance()
    {
        static Logger* instance = new Logger(Logger::Level::Verbose);
        return instance;
    };

    Logger(Level level);

    template<typename... Args>
    void Log(Level level, const char* string, Args&&... args)
    {
        char buffer[1024];
        snprintf(buffer, sizeof(buffer), string, std::forward<Args>(args)...);
        std::cout << buffer << std::endl;
    };

private:
    Level m_Level;
};

#ifdef BUILD_DESKTOP
#ifndef BUILD_TEST
#define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; } while (0);
#else
// #define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; /* TestSuite::GetInstance().AddLog(std::string(buffer)); */ } while (0);
#define LOG(...) do { Logger::GetInstance()->Log(__VA_ARGS__); } while(0);
#endif
#define LOGD(...) LOG(Logger::Level::Debug, __VA_ARGS__)
#define LOGI(...) LOG(Logger::Level::Info, __VA_ARGS__)
#define LOGE(...) LOG(Logger::Level::Error, __VA_ARGS__)
#endif
