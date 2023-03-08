#include "KumaAnimation.h"
#include "Component\SpriteComponent.h"

CKumaAnimation::CKumaAnimation()    :
    m_bIsAttack(false),
    m_bIsDie(false),
    m_bIsHit(false),
    m_bIsUltimate(false),
    m_bIsEarthHit(false),
    m_bIsEarthHitReady(false),
    m_bIsIntro(false),
    m_bIsStamping(false),
    m_bIsJumpStamping(false)
{
}

CKumaAnimation::CKumaAnimation(const CKumaAnimation& anim)
{
}

CKumaAnimation::~CKumaAnimation()
{
}

void CKumaAnimation::SetIntro(bool Value)
{
    m_bIsIntro = Value;
}

void CKumaAnimation::SetEarthHitReady(bool Value)
{
    m_bIsEarthHitReady = Value;
}

void CKumaAnimation::SetEarthHit(bool Value)
{
    m_bIsEarthHit = Value;
}

void CKumaAnimation::SetUltimate(bool Value)
{
    m_bIsUltimate = Value;
}

void CKumaAnimation::SetDie(bool Value)
{
    m_bIsDie = Value;
}

void CKumaAnimation::SetAttack(bool Value)
{
    m_bIsAttack = Value;
}

void CKumaAnimation::SetHit(bool Value)
{
    m_bIsHit = Value;
}

void CKumaAnimation::SetStamping(bool Value)
{
    m_bIsStamping = Value;
}

void CKumaAnimation::SetJumpStamping(bool Value)
{
    m_bIsJumpStamping = Value;
}

void CKumaAnimation::SetShield(bool Value)
{
    m_bIsShield = Value;
}

void CKumaAnimation::SetShieldDash(bool Value)
{
    m_bIsShieldDash = Value;
}

void CKumaAnimation::SetShieldDashLoop(bool Value)
{
    m_bIsShieldDashLoop = Value;
}

void CKumaAnimation::Start()
{
    CAnimation2D::Start();
}

bool CKumaAnimation::Init()
{
    CAnimation2D::Init();

    AddAnimationSequence2D("KumaIntro", false);
    AddAnimationSequence2D("KumaIdle");
    AddAnimationSequence2D("KumaStampingUp", false);
    AddAnimationSequence2D("KumaStampingHit", false);
    SetNextSequence2D("KumaStampingUp", "KumaStampingHit");
    AddAnimationSequence2D("KumaStampingJumpUp", false);
    AddAnimationSequence2D("KumaStampingReady",false);
    AddAnimationSequence2D("KumaWhirlWindLoop");
    AddAnimationSequence2D("KumaWhirlWindReady", false);
    SetNextSequence2D("KumaWhirlWindReady", "KumaWhirlWindLoop");
    AddAnimationSequence2D("KumaEarthQuakeAttack",false);
    AddAnimationSequence2D("KumaEarthQuakeHit", false);
    AddAnimationSequence2D("KumaEarthQuakeJump", false);
    AddAnimationSequence2D("KumaEarthQuakeReady", false);
    AddAnimationSequence2D("KumaDie",false);
    AddAnimationSequence2D("KumaEarthQuakeJumpReady",false);
    AddAnimationSequence2D("KumaJumpStampingJumpUp",false);
    AddAnimationSequence2D("KumaJumpStampingHit",false);
    AddAnimationSequence2D("KumaShieldReady",false);
    AddAnimationSequence2D("KumaTackleReady",false);
    AddAnimationSequence2D("KumaTackle");
    SetNextSequence2D("KumaTackleReady", "KumaTackle");

    return true;
}

void CKumaAnimation::PostUpdate(float DeltaTime)
{
    CAnimation2D::PostUpdate(DeltaTime);

    if (m_bIsShieldDashLoop)
    {
        ChangeAnimation("KumaTackle");
    }
    else if (m_bIsShieldDash)
    {
        ChangeAnimation("KumaTackleReady");
    }
    else if (m_bIsShield)
    {
        ChangeAnimation("KumaShieldReady");
    }
    else if (m_bIsJumpStamping)
    {
        if(m_Owner->IsGround())
            ChangeAnimation("KumaJumpStampingHit");
        else
            ChangeAnimation("KumaJumpStampingJumpUp");
    }
    else if (m_bIsStamping)
    {
        if (GetCurrentSequenceName() != "KumaStampingHit")
            ChangeAnimation("KumaStampingUp");

        else
            ChangeAnimation("KumaStampingHit");

    }
    else if (m_bIsUltimate)
    {
        if (GetCurrentSequenceName() != "KumaWhirlWindLoop")
            ChangeAnimation("KumaWhirlWindReady");
        else
            ChangeAnimation("KumaWhirlWindLoop");

    }
    else if (m_bIsEarthHitReady)
    {
        ChangeAnimation("KumaEarthQuakeJumpReady");
    }
    else if (m_bIsEarthHit)
    {
        if (m_Owner->IsFalling())
            ChangeAnimation("KumaEarthQuakeJump");
        else if (m_Owner->IsJump())
            ChangeAnimation("KumaStampingJumpUp");

        else
            ChangeAnimation("KumaEarthQuakeHit");
    }
    else if (m_bIsIntro)
        ChangeAnimation("KumaIntro");
    else
        ChangeAnimation("KumaIdle");
}
