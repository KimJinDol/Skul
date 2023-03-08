#include "SkulAnimation.h"
#include "Component/SpriteComponent.h"

CSkulAnimation::CSkulAnimation()	:
	m_AttackComboCount(0),
	m_bIsJumpAttack(false),
	m_bIsDash(false),
	m_bIsSkill1(false),
	m_bIsEvent(false)
{
}

CSkulAnimation::CSkulAnimation(const CSkulAnimation& anim)
{
}

CSkulAnimation::~CSkulAnimation()
{
	SAFE_RELEASE(m_pSkul);
}

void CSkulAnimation::SetEventMode(bool Value)
{
	m_bIsEvent = Value;
}

void CSkulAnimation::SetIlseom(bool Value)
{
	m_bIsIlseom = Value;
}

void CSkulAnimation::SetSwitch(bool Value)
{
	m_bIsSwitch = Value;
}

void CSkulAnimation::SetSkill2(bool Value)
{
	m_bIsSkill2 = Value;
}

void CSkulAnimation::SetSkill1(bool Value)
{
	m_bIsSkill1 = Value;
}

void CSkulAnimation::SetAttackComboCount(int Count)
{
	m_AttackComboCount = Count;
}

void CSkulAnimation::SetJumpAttackOn(bool Value)
{
	m_bIsJumpAttack = Value;
}

void CSkulAnimation::SetDash(bool Value)
{
	m_bIsDash = Value;
}

void CSkulAnimation::Start()
{
	CAnimation2D::Start();
}

bool CSkulAnimation::Init()
{
	if (!CAnimation2D::Init())
		return false;

	m_pSkul = (CSkul*)m_Owner->GetOwner();

	AddAnimationSequence2D("SkulIdle");
	AddAnimationSequence2D("SkulRun");
	AddAnimationSequence2D("SkulJumpUp");
	AddAnimationSequence2D("SkulJumpDownStart", false);
	AddAnimationSequence2D("SkulJumpDownLoop");
	AddAnimationSequence2D("SkulDash", false);
	SetNextSequence2D("SkulJumpDownStart", "SkulJumpDownLoop");
	AddAnimationSequence2D("SkulAttack1", false);
	AddAnimationSequence2D("SkulAttack2", false);
	AddAnimationSequence2D("SkulJumpAttack", false);
	AddAnimationSequence2D("SkulSwitch");
	AddAnimationSequence2D("SkulSkill1", false);
	AddAnimationSequence2D("SkulSkill2", false);

    AddAnimationSequence2D("SkulIdle_NoHead");
    AddAnimationSequence2D("SkulRun_NoHead");
    AddAnimationSequence2D("SkulJumpUp_NoHead");
    AddAnimationSequence2D("SkulJumpDownStart_NoHead", false);
    AddAnimationSequence2D("SkulJumpDownLoop_NoHead");
    AddAnimationSequence2D("SkulDash_NoHead", false);
    SetNextSequence2D("SkulJumpDownStart_NoHead", "SkulJumpDownLoop_NoHead");
    AddAnimationSequence2D("SkulAttack1_NoHead", false);
    AddAnimationSequence2D("SkulAttack2_NoHead", false);
	AddAnimationSequence2D("SkulJumpAttack_NoHead", false);

	AddAnimationSequence2D("SkulIdle_ChiefGuard");
	AddAnimationSequence2D("SkulRun_ChiefGuard");
	AddAnimationSequence2D("SkulAttack1_ChiefGuard", false);
	AddAnimationSequence2D("SkulAttack2_ChiefGuard", false);
	AddAnimationSequence2D("SkulAttack3_ChiefGuard", false);
	AddAnimationSequence2D("SkulJumpAttack_ChiefGuard", false);
	AddAnimationSequence2D("SkulJumpUp_ChiefGuard");
	AddAnimationSequence2D("SkulJumpDownStart_ChiefGuard", false);
	AddAnimationSequence2D("SkulJumpDownLoop_ChiefGuard");
	SetNextSequence2D("SkulJumpDownStart_ChiefGuard", "SkulJumpDownLoop_ChiefGuard");
	AddAnimationSequence2D("SkulDash_ChiefGuard", false);
	AddAnimationSequence2D("SkulSkill1_ChiefGuard", false);
	AddAnimationSequence2D("SkulSkill1_Slash_ChiefGuard", false);
	SetNextSequence2D("SkulSkill1_ChiefGuard", "SkulSkill1_Slash_ChiefGuard");
	AddAnimationSequence2D("SkulSkill2_ChiefGuard", false);
	AddAnimationSequence2D("SkulSwitch_ChiefGuard", false);


	return true;
}

void CSkulAnimation::Update(float DeltaTime)
{
	CAnimation2D::Update(DeltaTime);

	if (m_bIsEvent) return;

	char	AnimName[256] = {};
	bool	CanChange = true;

	if (m_bIsSwitch)
		sprintf_s(AnimName, "SkulSwitch");

	else if(m_bIsSkill2)
		sprintf_s(AnimName, "SkulSkill2");

	else if (m_bIsSkill1)
		sprintf_s(AnimName, "SkulSkill1");
	
    else if (m_bIsDash)
        sprintf_s(AnimName, "SkulDash");

	else if (m_pSkul->IsJump())
	{
		if (m_Owner->IsFalling())
		{
            if (GetCurrentSequenceName() != "SkulJumpDownLoop")
                sprintf_s(AnimName, "SkulJumpDownStart");
		}
        else
            sprintf_s(AnimName, "SkulJumpUp");
	}
	else if (m_pSkul->IsAttack())
		sprintf_s(AnimName, "SkulAttack%d", m_AttackComboCount);

	else if (m_bIsJumpAttack)
		sprintf_s(AnimName, "SkulJumpAttack");

	else
	{
        if (m_Owner->IsFalling())
            sprintf_s(AnimName, "SkulJumpDownStart");
        else if (m_Owner->GetMove().x != 0.f)
            sprintf_s(AnimName, "SkulRun");
        else
            sprintf_s(AnimName, "SkulIdle");
	}

	ESkulState State = m_pSkul->GetSkulState();

	switch (State)
	{
	case ESkulState::NoHead:
		strcat_s(AnimName, "_NoHead");
		break;
	case ESkulState::ChiefGuard:
		if (m_bIsIlseom)
			CanChange = false;
		strcat_s(AnimName, "_ChiefGuard");
		break;
	}

	if (CanChange)
		ChangeAnimation(AnimName);
}
