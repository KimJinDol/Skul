#pragma once

#include "GameEngine.h"

struct TimerInfo
{
    std::string				Name;
    float					Second;			// rest time
    float					RestSecond;			// rest time
    std::function<void()>   Function;		// callBack
    class CRef*             Owner;
};

class CTimer
{
public:
    CTimer();
    ~CTimer();

private:
    LARGE_INTEGER   m_Second;
    LARGE_INTEGER   m_Time;
    float           m_DeltaTime;
    float           m_FPS;
    float           m_FPSTime;
    int             m_Tick;
    bool            m_FPSEnable;

    // Timer
    std::list<TimerInfo*>   m_TimerList;


public:
    float GetTimerRatio(const std::string& Name)
    {
        TimerInfo* Timer = FindTimer(Name);

        if (!Timer) return -1.f;

        return Timer->RestSecond / Timer->Second;
    }

    float GetTimerRestSecond(const std::string& Name)
    {
        TimerInfo* Timer = FindTimer(Name);

        if (!Timer) return -1.f;

        return Timer->RestSecond;
    }

    float GetDeltaTime()    const
    {
        return m_DeltaTime;
    }

    float GetFPS()  const
    {
        return m_FPS;
    }

    bool IsFPS()    const
    {
        return m_FPSEnable;
    }

public:
    bool Init();
    void Update();

public:
    TimerInfo* FindTimer(const std::string& Name);
    std::list<TimerInfo*>::iterator GetTimerIterator(const std::string& Name, CRef* Owner);
    void RemoveTimer(const std::string& Name, CRef* Owner)
    {
        std::list<TimerInfo*>::iterator iter = GetTimerIterator(Name, Owner);

        if (iter == m_TimerList.end()) return;

        delete* iter;
        m_TimerList.erase(iter);
    }
    void RemoveAllTimer(CRef* Owner)
    {
        auto iter = m_TimerList.begin();
        auto iterEnd = m_TimerList.end();

        for (; iter != iterEnd;)
        {
            if ((*iter)->Owner == Owner)
            {
                delete* iter;
                iter = m_TimerList.erase(iter);
                iterEnd = m_TimerList.end();

                continue;
            }
            iter++;
        }
    }

    void ClearTimer()
    {
        auto iter = m_TimerList.begin();
        auto iterEnd = m_TimerList.end();

        for (; iter != iterEnd; iter++)
            delete* iter;

        m_TimerList.clear();
    }

public:
    template <typename T>
    void StartTimer(const std::string& Name, float Second,
        T* pObj, void (T::* pFunc)())
    {
        TimerInfo* pInfo = new TimerInfo();
        pInfo->Name = Name;
        pInfo->Function = std::bind(pFunc, pObj);
        pInfo->Second = Second;
        pInfo->RestSecond = Second;
        pInfo->Owner = pObj;
        m_TimerList.push_back(pInfo);
    }
};

