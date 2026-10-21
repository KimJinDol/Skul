#pragma once
#include "AITask.h"
class CAITask_Attack :
    public CAITask
{
public:
    CAITask_Attack();
    virtual ~CAITask_Attack();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

