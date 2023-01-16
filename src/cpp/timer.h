#pragma once

#include <time.h>

#include "util.h"

#ifdef _WIN32
#include <Windows.h>
#include <sysinfoapi.h>

#define CLOCK_MONOTONIC 0

int clock_gettime(int, struct timespec* spec) {
    __int64 wintime; GetSystemTimeAsFileTime((FILETIME*)&wintime);
    wintime -= 116444736000000000i64;
    spec->tv_sec = wintime / 10000000i64;           // seconds
    spec->tv_nsec = wintime % 10000000i64 * 100;    // nano-seconds
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
    ScopeTimer();
    ~ScopeTimer();
private:
    BasicTimer m_Timer;
};

class ITimer {
    virtual float GetTotalTime() const = 0;
    virtual float GetDelta() const = 0;
    virtual long long GetTime() const = 0;
    virtual void Update() = 0;
    virtual void Reset() = 0;
    virtual void Start() = 0;
    virtual void Pause() = 0;
};

class Timer : ITimer
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
