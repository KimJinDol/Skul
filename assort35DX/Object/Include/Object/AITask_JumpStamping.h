#pragma once
#include "AITask.h"
class CAITask_JumpStamping :
    public CAITask
{
public:
    CAITask_JumpStamping();
    virtual ~CAITask_JumpStamping();

private:
    float m_CurMoveTime;
public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

