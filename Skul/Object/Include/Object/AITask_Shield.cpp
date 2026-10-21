#include "AITask_Shield.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_Shield::CAITask_Shield()
{
}

CAITask_Shield::~CAITask_Shield()
{
}

void CAITask_Shield::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;


    if (!pController->GetBoolData("IsShield"))
    {
        pController->SetBoolData("IsShield", true);
        //pController->SetFloatData("JumpStampingInitPos", Kuma->GetWorldPos().x);
        Kuma->Shield(false);
    }
}
