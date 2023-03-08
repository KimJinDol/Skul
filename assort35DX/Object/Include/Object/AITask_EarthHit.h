#pragma once
#include "AITask.h"

class CAITask_EarthHit :
    public CAITask
{
public:
    CAITask_EarthHit();
    virtual ~CAITask_EarthHit();

    float   m_JumpDuration; // 궁 지속 시간
    float   m_CurJumpDuration; // 궁 지속 시간
    float   m_MoveSpeed; // 타겟 따라가는 속도

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

