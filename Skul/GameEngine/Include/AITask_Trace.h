#pragma once
#include "AITask.h"

class CAITask_Trace :
    public CAITask
{
public:
    CAITask_Trace();
    virtual ~CAITask_Trace();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

