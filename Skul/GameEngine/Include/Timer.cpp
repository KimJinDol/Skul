#include "Timer.h"
#include "Engine.h"

CTimer::CTimer()    :
    m_Second{},
    m_Time{},
    m_DeltaTime(0.f),
    m_FPS(0.f),
    m_FPSTime(0.f),
    m_Tick(0),
    m_FPSEnable(false)
{
}

CTimer::~CTimer()
{
    auto iter = m_TimerList.begin();
    auto iterEnd = m_TimerList.end();

    for (; iter != iterEnd; iter++)
    {
        delete* iter;
    }

    m_TimerList.clear();
}

bool CTimer::Init()
{
    QueryPerformanceFrequency(&m_Second);
    QueryPerformanceCounter(&m_Time);

    return true;
}

void CTimer::Update()
{
    m_FPSEnable = false;

    LARGE_INTEGER   Time = {};
    QueryPerformanceCounter(&Time);

    m_DeltaTime = (Time.QuadPart - m_Time.QuadPart) / (float)m_Second.QuadPart;

    m_Time = Time;

    m_FPSTime += m_DeltaTime;
    ++m_Tick;

    if (m_Tick == 60)
    {
        m_FPS = m_Tick / m_FPSTime;
        m_Tick = 0;
        m_FPSTime = 0.f;

        m_FPSEnable = true;
    }

    // Timer
    std::list<TimerInfo*>::iterator iter = m_TimerList.begin();
    std::list<TimerInfo*>::iterator iterEnd = m_TimerList.end();

    for (; iter != iterEnd;)
    {
        (*iter)->RestSecond -= m_DeltaTime;
        if ((*iter)->RestSecond <= 0)
        {
            (*iter)->Function();
            delete* iter;
            iter = m_TimerList.erase(iter);
            iterEnd = m_TimerList.end();
            continue;
        }

        ++iter;
    }
}

TimerInfo* CTimer::FindTimer(const std::string& Name)
{
    std::list<TimerInfo*>::iterator iter = m_TimerList.begin();
    std::list<TimerInfo*>::iterator iterEnd = m_TimerList.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->Name == Name)
        {
            return (*iter);
        }
    }

    return nullptr;
}

std::list<TimerInfo*>::iterator CTimer::GetTimerIterator(const std::string& Name, CRef* Owner)
{
    std::list<TimerInfo*>::iterator iter = m_TimerList.begin();
    std::list<TimerInfo*>::iterator iterEnd = m_TimerList.end();

    for (; iter != iterEnd; iter++)
    {
        if ((*iter)->Name == Name && (*iter)->Owner == Owner)
        {
            return iter;
        }
    }

    return iterEnd;
}
