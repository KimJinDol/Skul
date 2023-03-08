#include "AITask_Trace.h"
#include "GameObject.h"
#include "AIController.h"
#include "AICharacter.h"

CAITask_Trace::CAITask_Trace()
{
}

CAITask_Trace::~CAITask_Trace()
{
}

void CAITask_Trace::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CGameObject* pTarget = pController->GetGameObjectData("Target");

    if (!pTarget)
        return;

    Vector2 TargetPos = pTarget->GetWorldPos2D();

    Vector2 OwnerPos = pOwner->GetWorldPos2D();

    Vector2 MoveDir;
    MoveDir.x = TargetPos.x - OwnerPos.x;
    
    MoveDir.Normalize();

    pOwner->AddMove(MoveDir.x * pOwner->GetMoveSpeed() * DeltaTime, 0.f, 0.f);
}
