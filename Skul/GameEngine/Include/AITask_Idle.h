#pragma once
#include "AITask.h"

class CAITask_Idle :
    public CAITask
{
public:
    CAITask_Idle();
    virtual ~CAITask_Idle();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

