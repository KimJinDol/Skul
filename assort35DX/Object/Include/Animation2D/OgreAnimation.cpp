#include "OgreAnimation.h"

COgreAnimation::COgreAnimation()	:
	m_bIsAttack(false)
{
}

COgreAnimation::COgreAnimation(const COgreAnimation& anim)
{
}

COgreAnimation::~COgreAnimation()
{
}

void COgreAnimation::SetDie(bool Value)
{
	m_bIsDie = Value;
}

void COgreAnimation::SetAttack(bool Value)
{
	m_bIsAttack = Value;
}

void COgreAnimation::Start()
{
	CAnimation2D::Start();
}

bool COgreAnimation::Init()
{
	if (!CAnimation2D::Init())
		return false;

	m_pOgre = (COgreAI*)m_Owner->GetOwner();

	AddAnimationSequence2D("OgreIdle");
	AddAnimationSequence2D("OgreRun");
	AddAnimationSequence2D("OgreAttack", false);
	AddAnimationSequence2D("OgreDie", false);

	return true;
}

void COgreAnimation::PostUpdate(float DeltaTime)
{
	CAnimation2D::PostUpdate(DeltaTime);

	if(m_bIsDie)
		ChangeAnimation("OgreDie");

	else if (m_bIsAttack)
		ChangeAnimation("OgreAttack");

	else if (m_Owner->GetMove().x != 0.f)
		ChangeAnimation("OgreRun");

	else
		ChangeAnimation("OgreIdle");
}
