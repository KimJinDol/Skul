#include "AISelectWarrior.h"
#include "AIController.h"
#include "AICharacter.h"
#include "BasicMath.h"
#include <random>

CAISelectWarrior::CAISelectWarrior()
{
    m_AttackDistanceMax = 100;
    m_AttackDistanceMin = 30;
}

CAISelectWarrior::~CAISelectWarrior()
{
}

bool CAISelectWarrior::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    CGameObject* Target = pController->GetGameObjectData("Target");
    float Distance = Target->GetWorldPos2D().Distance(pOwner->GetWorldPos2D());

    if (pController->GetBoolData("IsStun"))
    {
        pController->ChangeAI("Stun");
    }

    else if (pController->GetBoolData("IsStanby"))
        pController->ChangeAI("Idle");

    else if (Distance <= pController->GetFloatData("TraceDistance") && Target->GetWorldPos().y >= pOwner->GetWorldPos().y )
    {
        if (Distance <= pController->GetFloatData("AttackDistance") || pController->GetBoolData("IsAttack"))
        {
            pController->ChangeAI("Attack");

            m_AttackDistanceRand = false;
        }
        else
        {
            pController->ChangeAI("Trace");
            if (!m_AttackDistanceRand)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dis(m_AttackDistanceMin, m_AttackDistanceMax);
                pController->SetFloatData("AttackDistance", dis(gen));

                m_AttackDistanceRand = true;
            }
        }
    }
    else
        pController->ChangeAI("Idle");

    return true;
}
