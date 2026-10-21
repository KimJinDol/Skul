#pragma once
#include "AITask.h"

class CAITask_Stun :
    public CAITask
{
public:
    CAITask_Stun();
    virtual ~CAITask_Stun();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

