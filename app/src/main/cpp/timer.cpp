#include "timer.h"

Timer::Timer() :
    m_stoppedTime(0.f),
    m_delta(0.f),
    m_isPaused(false)
{ }

float Timer::GetTime() const {
    if (m_isPaused) {
        return Timer::Diff(m_startTime, m_pauseTime) - m_stoppedTime;
    } else {
        return Timer::Diff(m_startTime, m_currentTime) - m_stoppedTime;
    }
}

float Timer::GetDelta() const {
    return m_delta;
}

void Timer::Update() {
    if (m_isPaused) {
        m_delta = 0.f;
        return;
    }

    m_currentTime = GetCurrentTime();
    m_delta = Timer::Diff(m_prevTime, m_currentTime);

    m_prevTime = m_currentTime;

    if (m_delta < 0.f) {
        m_delta = 0.f;
    }
}

void Timer::Reset() {
    m_currentTime = GetCurrentTime();

    m_startTime = m_currentTime;
    m_prevTime = m_currentTime;

    m_delta = 0.f;
    m_stoppedTime = 0.f;
    m_isPaused = false;
}

void Timer::Start()
{
    if (m_isPaused) {
        m_currentTime = GetCurrentTime();

        m_stoppedTime += Timer::Diff(m_pauseTime, m_currentTime);
        m_startTime = m_currentTime;
        m_prevTime = m_currentTime;
        m_isPaused = false;
        m_delta = 0.0f;
    }
}

void Timer::Pause()
{
    if (!m_isPaused)
    {
        m_currentTime = GetCurrentTime();
        m_pauseTime = m_currentTime;
        m_isPaused = true;
    }
}

timespec Timer::GetCurrentTime()
{
    timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        THROW("clock_gettime failed");
    }
    return ts;
}