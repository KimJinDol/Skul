#include "ArcherAnimation.h"
#include "Component/SpriteComponent.h"
#include <random>

CArcherAnimation::CArcherAnimation()
{
}

CArcherAnimation::CArcherAnimation(const CArcherAnimation& anim)
{
}

CArcherAnimation::~CArcherAnimation()
{
}

void CArcherAnimation::SetDie(bool Value)
{
    m_bIsDie = Value;
}

void CArcherAnimation::SetAttack(bool Value)
{
    m_bIsAttack = Value;
}

void CArcherAnimation::SetHit(bool Value)
{
    m_bIsHit = Value;
}

void CArcherAnimation::Start()
{
    CAnimation2D::Start();
}

bool CArcherAnimation::Init()
{
    if (!CAnimation2D::Init())
        return false;

    AddAnimationSequence2D("ArcherIdle");
    AddAnimationSequence2D("ArcherRun");
    AddAnimationSequence2D("ArcherAttack", false);
    AddAnimationSequence2D("ArcherHit", false);
    AddAnimationSequence2D("ArcherHit2", false);
    AddAnimationSequence2D("ArcherDead", false);


    return true;
}

void CArcherAnimation::PostUpdate(float DeltaTime)
{
    CAnimation2D::PostUpdate(DeltaTime);

    if (m_bIsDie)
        ChangeAnimation("ArcherDead");

    else if (m_bIsHit)
    {
        char	AnimName[256] = {};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 2);
        int HitAnimType = dis(gen);

        sprintf_s(AnimName, "ArcherHit%d", HitAnimType);

        ChangeAnimation(AnimName);
    }


    else if (m_bIsAttack)
        ChangeAnimation("ArcherAttack");

    else if (m_Owner->GetMove().x != 0.f)
        ChangeAnimation("ArcherRun");

    else
        ChangeAnimation("ArcherIdle");
}
