#pragma once
#include "AITask.h"

class CAITask_Ultimate :
    public CAITask
{
public:
    CAITask_Ultimate();
    virtual ~CAITask_Ultimate();

    float   m_UltimateDuration; // �� ���� �ð�
    float   m_CurUltimateDuration; // �� ���� �ð�
    float   m_MoveSpeed; // Ÿ�� ���󰡴� �ӵ�

public:
    virtual void Run(class CAIController* pController, class CAICharacter* pOwner, class CScene* pScene, float DeltaTime);
};

