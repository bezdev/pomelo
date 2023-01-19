#include "timer.h"




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

BasicTimer::BasicTimer()
{
}

void BasicTimer::Start()
{
    clock_gettime(CLOCK_MONOTONIC, &m_StartTime);
}

float BasicTimer::GetTotalTime()
{
    clock_gettime(CLOCK_MONOTONIC, &m_CurrentTime);
    return (m_CurrentTime.tv_sec - m_StartTime.tv_sec) + (m_CurrentTime.tv_nsec - m_StartTime.tv_nsec) / 1e9f;
}

ScopeTimer::ScopeTimer(const char* name):
    m_Timer(BasicTimer()),
    m_Name(name)
{
    LOGD("ScopeTimerStart [%s]", m_Name);
    m_Timer.Start();
}

ScopeTimer::~ScopeTimer()
{
    LOGD("ScopeTimerEnd   [%s]: %f", m_Name, m_Timer.GetTotalTime());
}
