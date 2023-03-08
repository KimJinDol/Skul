#pragma once
#include "AITask.h"
class CAITask_Stamping :
    public CAITask
{
public:
    CAITask_Stamping();
    virtual ~CAITask_Stamping();

private:
    float   m_CurMoveTime;

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

