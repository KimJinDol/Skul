#pragma once
#include "AITask.h"
class CAITask_Intro :
    public CAITask
{
public:
    CAITask_Intro();
    virtual ~CAITask_Intro();

private:
    float m_CurMoveTime;

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);

};

