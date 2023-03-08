#include "AISelectKuma.h"
#include "AIController.h"
#include "AICharacter.h"
#include "Component\HPComponent.h"
#include <random>

CAISelectKuma::CAISelectKuma()  :
    m_PaternPercent(0.f),
    m_bIntroEnableCheck(false),
    m_bIntro(false)
{
    m_bPhase2 = false;
    m_bPhase3 = false;
}

CAISelectKuma::~CAISelectKuma()
{
}

bool CAISelectKuma::Run(CAIController* pController, CAICharacter* pOwner, CScene* pScene, float DeltaTime)
{
    if (pController->GetBoolData("IsStanby"))
        pController->ChangeAI("Idle");


    else
    {
        if (pController->GetBoolData("CanNewBehavior"))
        {
            CGameObject* Target = pController->GetGameObjectData("Target");

            if (m_bIntro && !m_bIntroEnableCheck)
            {
                m_bIntroEnableCheck = true;
                pController->SetEnable(false);
            }
            else if (!m_bIntro)
            {
                pController->ChangeAI("Intro");
                m_bIntro = true;
            }
            else if (pOwner->GetHPComponent()->GetHpRatio() <= 0.7f && !m_bPhase2)
            {
                pController->ChangeAI("EarthHit");
                m_bPhase2 = true;

            }
            else if (pOwner->GetHPComponent()->GetHpRatio() <= 0.3f && !m_bPhase3)
            {
                pController->ChangeAI("Ultimate");
                m_bPhase3 = true;
            }
            else
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0.f, 100.f);
                m_PaternPercent = dis(gen);

                if (Target->GetWorldPos().Distance(pOwner->GetWorldPos()) <= 400.f)
                {
                    if (m_PaternPercent >= 0.f && m_PaternPercent < 25.f)
                        pController->ChangeAI("JumpStamping");
                    else if (m_PaternPercent >= 25.f && m_PaternPercent < 70.f)
                        pController->ChangeAI("Stamping");
                    else if (m_PaternPercent >= 50.f && m_PaternPercent < 85.f)
                        pController->ChangeAI("Shield");
                    else
                        pController->ChangeAI("Tackle");
                }
                else
                {
                    if (m_PaternPercent >= 0.f && m_PaternPercent < 60.f)
                        pController->ChangeAI("JumpStamping");
                    else if (m_PaternPercent >= 60.f && m_PaternPercent < 85.f)
                        pController->ChangeAI("Tackle");
                    else
                        pController->ChangeAI("Shield");

                }
            }
            pController->SetBoolData("CanNewBehavior", false);
        }
    }
    return true;
}
