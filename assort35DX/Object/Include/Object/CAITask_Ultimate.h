#pragma once
#include "AITask.h"

class CAITask_Ultimate :
    public CAITask
{
public:
    CAITask_Ultimate();
    virtual ~CAITask_Ultimate();

    float   m_UltimateDuration; // 궁 지속 시간
    float   m_CurUltimateDuration; // 궁 지속 시간
    float   m_MoveSpeed; // 타겟 따라가는 속도

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

