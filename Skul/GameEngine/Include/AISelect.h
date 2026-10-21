#pragma once

#include "GameEngine.h"

class CAISelect
{
public:
    CAISelect();
    virtual ~CAISelect() = 0;

protected:
    std::string m_Name;
    float   m_DelayTime;
    float   m_DelayTimeMax;

    float   m_AttackDelayTime;
    float   m_AttackDelayTimeMax;

public:
    void SetAttackDelayTime(float Time)
    {
        m_AttackDelayTimeMax = Time;
    }

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

    void SetDelayTime(float Time)
    {
        m_DelayTimeMax = Time;
    }

    const std::string& GetName()  const
    {
        return m_Name;
    }

public:
    virtual bool Init();
    virtual bool Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime) = 0;
};

