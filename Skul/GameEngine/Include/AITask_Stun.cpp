#include "AITask_Stun.h"
#include "AIController.h"
#include "GameObject.h"
#include "AICharacter.h"
#include "BasicMath.h"

CAITask_Stun::CAITask_Stun()
{
}

CAITask_Stun::~CAITask_Stun()
{
}

void CAITask_Stun::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    Vector2 OwnerPos = pOwner->GetWorldPos2D();
    float PushOutMove = pController->GetFloatData("PushOutMove");
    float LerpPos = BasicMath::FInterpTo(OwnerPos.x, PushOutMove, DeltaTime, 5.f);
        
    pOwner->AddMove(LerpPos- OwnerPos.x, 0.f, 0.f);
}
