#include "CAITask_Ultimate.h"
#include "AIController.h"
#include "AICharacter.h"
#include "../Animation2D/KumaAnimation.h"
#include "KumaAI.h"
#include <math.h>

CAITask_Ultimate::CAITask_Ultimate()    :
    m_UltimateDuration(4.f),
    m_CurUltimateDuration(0.f),
    m_MoveSpeed(300.f)
{
}

CAITask_Ultimate::~CAITask_Ultimate()
{
}

void CAITask_Ultimate::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;

    if (!pController->GetBoolData("IsUltimate"))
    {
        pController->SetBoolData("IsUltimate", true);
        Kuma->Ultimate();
    }
    else
    {

        if (Kuma->IsUltimate())
        {
            m_CurUltimateDuration += DeltaTime;
            if (m_CurUltimateDuration >= m_UltimateDuration)
            {
                pController->SetBoolData("IsUltimate", false);
                Kuma->UltimateEnd();
                m_CurUltimateDuration = 0.f;
                return;
            }

            // Trace
            CGameObject* pTarget = pController->GetGameObjectData("Target");

            if (!pTarget)
                return;

            Vector2 TargetPos = pTarget->GetWorldPos2D();

            Vector2 OwnerPos = pOwner->GetWorldPos2D();

            Vector2 MoveDir;
            MoveDir.x = TargetPos.x - OwnerPos.x;

            if (fabs(MoveDir.x) <= 50.f)
                return;

            MoveDir.Normalize();

            pOwner->AddMove(MoveDir.x * pOwner->GetMoveSpeed() * DeltaTime, 0.f, 0.f);
        }
    }
}
