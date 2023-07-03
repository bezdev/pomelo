#pragma once

#include <time.h>

#include "util.h"

#ifdef _WIN32
#include <Windows.h>

#define CLOCK_MONOTONIC 0

class HighResolutionTimer {
public:
    HighResolutionTimer() {
        QueryPerformanceFrequency(&Frequency);
        QueryPerformanceCounter(&Start);
    }

    double Tick() {
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);
        LARGE_INTEGER interval;
        interval.QuadPart = end.QuadPart - Start.QuadPart;
        return static_cast<double>(interval.QuadPart) / Frequency.QuadPart;
    }

    LARGE_INTEGER Frequency;
    LARGE_INTEGER Start;
};

static inline int clock_gettime(int, struct timespec* spec) {
    static HighResolutionTimer instance;
    auto time = instance.Tick();
    // LARGE_INTEGER now;
    // QueryPerformanceCounter(&now);
    // LARGE_INTEGER interval;
    // interval.QuadPart = now.QuadPart - instance.Start.QuadPart;
    // auto time = static_cast<double>(interval.QuadPart) / instance.Frequency.QuadPart;
    spec->tv_sec = static_cast<time_t>(time);
    spec->tv_nsec = static_cast<long>((time - spec->tv_sec) * 1e9);
    return 0;
}
#endif

class BasicTimer {
public:
    BasicTimer();
    void Start();
    float GetTotalTime();

private:
    timespec m_StartTime;
    timespec m_CurrentTime;
};

class ScopeTimer {
public:
    ScopeTimer(const char* name);
    ~ScopeTimer();
private:
    ScopeTimer();
    BasicTimer m_Timer;
    const char* m_Name;
};

class Timer
{
public:
    Timer();

    float GetTotalTime() const;
    float GetDelta() const;
    long long GetTime() const;

    void Update();

    void Reset();
    void Start();
    void Pause();
private:
    void UpdateTotalTime();
    void UpdateDelta(long long delta);
    long long m_StartTime;
    long long m_PrevTime;
    long long m_CurrentTime;
    long long m_PauseTime;
    long long m_StoppedTime;
    float m_TotalTime;
    float m_Delta;
    bool m_IsPaused;
};

#define ENABLE_TIME_FUNC 0

#if ENABLE_TIME_FUNC
#define TIME_FUNC(line) do { { ScopeTimer SCOPE_TIMER_FUNCTION(#line); line; } } while(0);
#else
#define TIME_FUNC(line) line
#endif
