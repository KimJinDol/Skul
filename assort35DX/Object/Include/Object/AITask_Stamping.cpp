#include "AITask_Stamping.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_Stamping::CAITask_Stamping()    :
    m_CurMoveTime(0.f)
{
}

CAITask_Stamping::~CAITask_Stamping()
{
}

void CAITask_Stamping::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;


    if (!pController->GetBoolData("IsStamping"))
    {
        pController->SetBoolData("IsStamping", true);
        m_CurMoveTime = 0.f;
        pController->SetFloatData("StampingInitMove", Kuma->GetWorldPos().x);
        Kuma->Stamping();
    }
    else
    {
        if (pController->GetBoolData("IsStampingMove"))
        {
            float StampingInitPos = pController->GetFloatData("StampingInitMove");
            float StampingMovePos = StampingInitPos;
            float StampingMoveSpeed = pController->GetFloatData("StampingMoveSpeed");
            if (Kuma->GetView() == Object_View::Right)
                StampingMovePos += pController->GetFloatData("StampingMoveX");
            else
                StampingMovePos -= pController->GetFloatData("StampingMoveX");

            m_CurMoveTime += DeltaTime;
            float MoveX = BasicMath::FLerp(StampingInitPos, StampingMovePos, m_CurMoveTime / StampingMoveSpeed);

            Kuma->AddMove(MoveX - Kuma->GetWorldPos().x, 0.f, 0.f);
        }
    }
    
}
