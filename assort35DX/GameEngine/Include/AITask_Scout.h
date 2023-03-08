#pragma once
#include "AITask.h"
class CAITask_Scout :
    public CAITask
{
public:
    CAITask_Scout();
    virtual ~CAITask_Scout();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

