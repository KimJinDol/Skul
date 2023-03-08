#include "WarriorAnimation.h"
#include "Component/SpriteComponent.h"
#include <random>

CWarriorAnimation::CWarriorAnimation()
{
}

CWarriorAnimation::CWarriorAnimation(const CWarriorAnimation& anim)
{
}

CWarriorAnimation::~CWarriorAnimation()
{
}

void CWarriorAnimation::SetDie(bool Value)
{
    m_bIsDie = Value;
}

void CWarriorAnimation::SetAttack(bool Value)
{
    m_bIsAttack = Value;
}

void CWarriorAnimation::SetHit(bool Value)
{
    m_bIsHit = Value;
}

void CWarriorAnimation::Start()
{
    CAnimation2D::Start();
}

bool CWarriorAnimation::Init()
{
    if (!CAnimation2D::Init())
        return false;

    AddAnimationSequence2D("WarriorIdle");
    AddAnimationSequence2D("WarriorRun");
    AddAnimationSequence2D("WarriorAttack", false);
    AddAnimationSequence2D("WarriorHit", false);
    AddAnimationSequence2D("WarriorHit2", false);
    AddAnimationSequence2D("WarriorDead", false);


    return true;
}

void CWarriorAnimation::PostUpdate(float DeltaTime)
{
    CAnimation2D::PostUpdate(DeltaTime);

    if (m_bIsDie)
        ChangeAnimation("WarriorDead");

    else if (m_bIsHit)
    {
        char	AnimName[256] = {};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(1, 2);
        int HitAnimType = dis(gen);

        sprintf_s(AnimName, "WarriorHit%d", HitAnimType);

        ChangeAnimation(AnimName);
    }


    else if (m_bIsAttack)
        ChangeAnimation("WarriorAttack");

    else if (m_Owner->GetMove().x != 0.f)
        ChangeAnimation("WarriorRun");

    else
        ChangeAnimation("WarriorIdle");

}
