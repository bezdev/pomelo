#include "timer.h"

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


Timer::Timer() {
    LOGD("Timer::Timer");
    Reset();
}

float Timer::GetTotalTime() const {
    return m_TotalTime;
}

float Timer::GetDelta() const {
    return m_Delta;
}

long long Timer::GetTime() const {
    timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0)
    {
        THROW("clock_gettime failed");
    }

    static timespec base = ts;

    if ((ts.tv_nsec - base.tv_nsec) < 0) {
        ts.tv_sec = ts.tv_sec - base.tv_sec - 1;
        ts.tv_nsec = 1000000000 + ts.tv_nsec - base.tv_nsec;
    }
    else
    {
        ts.tv_sec = ts.tv_sec - base.tv_sec;
        ts.tv_nsec = ts.tv_nsec - base.tv_nsec;
    }

    return static_cast<long long>(ts.tv_sec * 1e9 + ts.tv_nsec);
}

void Timer::Update() {
    if (m_IsPaused) {
        m_Delta = 0.f;
        return;
    }

    m_PrevTime = m_CurrentTime;
    m_CurrentTime = GetTime();

    UpdateTotalTime();
    UpdateDelta(m_CurrentTime - m_PrevTime);

    if (m_Delta < 0.f) {
        m_Delta = 0.f;
    }
}

void Timer::Reset() {
    m_CurrentTime = GetTime();
    m_StartTime = m_CurrentTime;
    m_PrevTime = m_CurrentTime;
    m_StoppedTime = 0;
    m_IsPaused = false;

    UpdateTotalTime();
    m_Delta = 0.f;
}

void Timer::Start()
{
    if (m_IsPaused) {
        m_IsPaused = false;
        Update();

        m_StoppedTime += m_CurrentTime - m_PauseTime;
        m_PrevTime = m_CurrentTime;

        UpdateTotalTime();
        m_Delta = 0.f;
    }
}

void Timer::Pause()
{
    if (!m_IsPaused)
    {
        Update();
        m_PauseTime = m_CurrentTime;
        m_IsPaused = true;
    }
}

void Timer::UpdateTotalTime() {
    m_TotalTime = static_cast<float>((m_CurrentTime - m_StartTime - m_StoppedTime)/ 1e6);
}

void Timer::UpdateDelta(long long delta) {
    m_Delta = static_cast<float>(delta / 1e6);
}
