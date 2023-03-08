#pragma once
#include "AITask.h"
class CAITask_Tackle :
    public CAITask
{
public:
    CAITask_Tackle();
    virtual ~CAITask_Tackle();

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);

};

