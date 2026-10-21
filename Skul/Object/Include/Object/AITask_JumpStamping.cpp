#include "AITask_JumpStamping.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_JumpStamping::CAITask_JumpStamping() :
    m_CurMoveTime(0.f)
{
}

CAITask_JumpStamping::~CAITask_JumpStamping()
{
}

void CAITask_JumpStamping::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;


    if (!pController->GetBoolData("IsJumpStamping"))
    {
        pController->SetBoolData("IsJumpStamping", true);
        m_CurMoveTime = 0.f;
        pController->SetFloatData("JumpStampingInitPos", Kuma->GetWorldPos().x);
        Kuma->JumpStamping();
    }
    else
    {
        float StampingInitPos = pController->GetFloatData("JumpStampingInitPos");
        float TargetPos = pController->GetFloatData("JumpStampingTargetPos");
        float StampingMoveSpeed = pController->GetFloatData("JumpStampingMoveSpeed");

        m_CurMoveTime += DeltaTime;
        float MoveX = BasicMath::FLerp(StampingInitPos, TargetPos, m_CurMoveTime / StampingMoveSpeed);

        Kuma->AddMove(MoveX - Kuma->GetWorldPos().x, 0.f, 0.f);
    }
}
