#pragma once
#include "AITask.h"
class CAITask_Shield :
    public CAITask
{
public:
    CAITask_Shield();
    virtual ~CAITask_Shield();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);

};

