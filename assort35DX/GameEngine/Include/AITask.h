#pragma once

#include "GameEngine.h"

class CAITask
{
public:
    CAITask();
    virtual ~CAITask() = 0;

protected:
    std::string m_Name;
    //bool        m_isComplete; // 임무를 모두 수행했는지

public:
    //void SetComplete(bool Value)
    //{
    //    m_isComplete = Value;
    //}

    void SetName(const std::string& Name)
    {
        m_Name = Name;
    }

public:
    /* bool IsComplete()    const
     {
         return m_isComplete;
     }*/

    const std::string& GetName()  const
    {
        return m_Name;
    }

public:
    virtual bool Init();
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime) = 0;
};

