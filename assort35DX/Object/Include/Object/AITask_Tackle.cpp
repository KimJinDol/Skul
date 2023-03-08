#include "AITask_Tackle.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_Tackle::CAITask_Tackle()
{
}

CAITask_Tackle::~CAITask_Tackle()
{
}

void CAITask_Tackle::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;


    if (!pController->GetBoolData("IsTackle"))
    {
        pController->SetBoolData("IsTackle", true);
        //pController->SetFloatData("JumpStampingInitPos", Kuma->GetWorldPos().x);
        Kuma->Shield(true);
    }
}
