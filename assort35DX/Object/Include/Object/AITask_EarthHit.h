#pragma once
#include "AITask.h"

class CAITask_EarthHit :
    public CAITask
{
public:
    CAITask_EarthHit();
    virtual ~CAITask_EarthHit();

    float   m_JumpDuration; // �� ���� �ð�
    float   m_CurJumpDuration; // �� ���� �ð�
    float   m_MoveSpeed; // Ÿ�� ���󰡴� �ӵ�

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

