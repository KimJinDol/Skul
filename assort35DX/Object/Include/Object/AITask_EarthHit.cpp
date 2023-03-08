#include "AITask_EarthHit.h"
#include "AICharacter.h"
#include "KumaAI.h"
#include "BasicMath.h"

CAITask_EarthHit::CAITask_EarthHit()
{
    m_JumpDuration = .6f;
    m_CurJumpDuration = 0.f;
}

CAITask_EarthHit::~CAITask_EarthHit()
{
}

void CAITask_EarthHit::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CKumaAI* Kuma = (CKumaAI*)pOwner;
    if (!Kuma) return;

    if (!pController->GetBoolData("IsEarthHit"))
    {
        pController->SetBoolData("IsEarthHit", true);
        Kuma->EarthHit();
    }
    else
    {
        if (Kuma->IsEarthHit())
        {
            m_CurJumpDuration += DeltaTime;
            Vector2 Pos = pOwner->GetWorldPos2D();
            if (pOwner->IsGround() && !pController->GetBoolData("EarthHitJump"))
            {
                pController->SetFloatData("EarthHitJumpStartXPos", Pos.x);
                pController->SetBoolData("EarthHitJump", true);
                pController->SetBoolData("EarthHitCheck", false);
                Kuma->EarthHitJump();
                m_CurJumpDuration = 0.f;
            }
            else
            {
                // ¾À Áß¾ÓÀ¸·Î ÀÌµ¿
                float StartXPos = pController->GetFloatData("EarthHitJumpStartXPos");
                //float Lerp = BasicMath::FInterpTo(Pos.x, 640.f, DeltaTime, 2.f)
                float Lerp = BasicMath::FLerp(StartXPos, 2564.f, m_CurJumpDuration / m_JumpDuration);

                Lerp -= Pos.x;
                
                pOwner->AddMove(Lerp, 0.f, 0.f);

                if (pOwner->IsGround() && !pController->GetBoolData("EarthHitCheck"))
                {
                    pController->SetBoolData("EarthHitCheck", true);
                    Kuma->EarthHitAttackCheck();
                }

            }
        }
    }
}
