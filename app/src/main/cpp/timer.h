#pragma once

#include <time.h>
#include "util.h"

class Timer
{
public:
    Timer();

    float GetTime() const;
    float GetDelta() const;

    void Update();

    void Reset();
    void Start();
    void Pause();
private:
    timespec m_pauseTime;
    timespec m_prevTime;
    timespec m_startTime;
    timespec m_currentTime;
    float m_stoppedTime;

    float m_delta;

    bool m_isPaused;

    timespec GetCurrentTime();

    static float Diff(timespec start, timespec end) {
        timespec ts;
        if ((end.tv_nsec-start.tv_nsec) < 0 ) {
            ts.tv_sec = end.tv_sec-start.tv_sec-1;
            ts.tv_nsec = 1000000000 + end.tv_nsec-start.tv_nsec;
        } else {
            ts.tv_sec = end.tv_sec-start.tv_sec;
            ts.tv_nsec = end.tv_nsec-start.tv_nsec;
        }

        return static_cast<float>(ts.tv_sec * 1000 + ts.tv_nsec * 1.f / 1e6);
    }
};