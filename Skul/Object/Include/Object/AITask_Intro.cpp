#include "AITask_Intro.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_Intro::CAITask_Intro()  :
    m_CurMoveTime(0.f)
{
}

CAITask_Intro::~CAITask_Intro()
{
}

void CAITask_Intro::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;


    if (!pController->GetBoolData("IsIntro"))
    {
        pController->SetBoolData("IsIntro", true);
        m_CurMoveTime = 0.f;
        pController->SetFloatData("JumpStampingInitPos", Kuma->GetWorldPos().x);
        Kuma->IntroJumpStamping();
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
