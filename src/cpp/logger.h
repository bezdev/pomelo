#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
        Test,
        Debug,
        Info,
        Error
    };

    static Logger* GetInstance()
    {
        static Logger instance(Logger::Level::Verbose, "[BEZLOG]");
        return &instance;
    };

    Logger(Level level, const char* name);

    template<typename... Args>
    void Log(Level level, const char* string, Args&&... args)
    {
        if (level < m_Level) return;

        char buffer[1024];
        snprintf(buffer, sizeof(buffer), string, std::forward<Args>(args)...);

        std::basic_stringstream<char> ss;
#ifdef BUILD_TEST
        ss << "  ";
#endif
        ss << m_Name << GetTag(level) << buffer << std::endl;

        std::string logString = ss.str();
        std::cout << logString;

#ifdef BUILD_TEST
        m_Log.push_back(logString);
#endif
    };

    std::vector<std::string> GetLog() { return m_Log; };
    void Clear() { m_Log.clear(); };

private:
    const char* GetTag(Level level);
    std::vector<std::string> m_Log;
    Level m_Level;
    const char* m_Name;
};

#ifdef BUILD_DESKTOP
// #ifndef BUILD_TEST
// #define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; } while (0);
// #else
// #define LOG(...) do { char buffer[1000]; snprintf(buffer, sizeof(buffer), __VA_ARGS__); std::cout << buffer << std::endl; /* TestSuite::GetInstance().AddLog(std::string(buffer)); */ } while (0);
#define LOG(...) do { Logger::GetInstance()->Log(__VA_ARGS__); } while(0);
// #endif
#define LOGT(...) LOG(Logger::Level::Test, __VA_ARGS__)
#define LOGD(...) LOG(Logger::Level::Debug, __VA_ARGS__)
#define LOGI(...) LOG(Logger::Level::Info, __VA_ARGS__)
#define LOGE(...) LOG(Logger::Level::Error, __VA_ARGS__)
#endif
