#pragma once

#include <time.h>

#include "util.h"

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
