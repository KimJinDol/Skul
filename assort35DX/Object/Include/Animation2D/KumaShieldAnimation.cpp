#include "KumaShieldAnimation.h"

CKumaShieldAnimation::CKumaShieldAnimation() :
    m_bIsOn(false)
{
}

CKumaShieldAnimation::CKumaShieldAnimation(const CKumaShieldAnimation& anim)
{
}

CKumaShieldAnimation::~CKumaShieldAnimation()
{
}

void CKumaShieldAnimation::SetOn(bool Value)
{
    m_bIsOn = Value;
}

void CKumaShieldAnimation::Start()
{
    CAnimation2D::Start();
}

bool CKumaShieldAnimation::Init()
{
    if (!CAnimation2D::Init())
        return false;

    AddAnimationSequence2D("KumaShieldOn", false);
    AddAnimationSequence2D("KumaShieldOff", false, true);
    AddAnimationSequence2D("KumaShieldLoop");
    SetNextSequence2D("KumaShieldOn", "KumaShieldLoop");

    return true;
}

void CKumaShieldAnimation::PostUpdate(float DeltaTime)
{
    CAnimation2D::PostUpdate(DeltaTime);

    if (m_bIsOn)
    {
        if (GetCurrentSequenceName() != "KumaShieldLoop")
            ChangeAnimation("KumaShieldOn");
    }
    else
    {
        ChangeAnimation("KumaShieldOff");
    }
}
