#include "AISelectOgre.h"
#include "AIController.h"
#include "AICharacter.h"

CAISelectOgre::CAISelectOgre()
{
}

CAISelectOgre::~CAISelectOgre()
{
}

bool CAISelectOgre::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    float Distance = pController->GetGameObjectData("Target")->GetWorldPos2D().Distance(pOwner->GetWorldPos2D());

    if (pController->GetBoolData("IsStanby"))
        pController->ChangeAI("Idle");

    else if (Distance <= pController->GetFloatData("TraceDistance"))
    {
        if (Distance <= pController->GetFloatData("AttackDistance") || pController->GetBoolData("IsAttack"))
        {
            pController->ChangeAI("Attack");
        }
        else
        {
            pController->ChangeAI("Trace");
        }
    }
    else
        pController->ChangeAI("Idle");

    return true;
}
