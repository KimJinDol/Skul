#pragma once
#include "Resource\Animation2D.h"

class CKumaAnimation :
    public CAnimation2D
{
	friend class CSpriteComponent;

protected:
	CKumaAnimation();
	CKumaAnimation(const CKumaAnimation& anim);
	virtual ~CKumaAnimation();

private:

	bool	m_bIsAttack;
	bool	m_bIsDie;
	bool	m_bIsHit;
	bool	m_bIsUltimate;
	bool	m_bIsEarthHitReady;
	bool	m_bIsEarthHit;
	bool	m_bIsIntro;
	bool	m_bIsStamping;
	bool	m_bIsJumpStamping;
	bool	m_bIsShield;
	bool	m_bIsShieldDash;
	bool	m_bIsShieldDashLoop;


public:
	void SetIntro(bool Value);
	void SetEarthHitReady(bool Value);
	void SetEarthHit(bool Value);
	void SetUltimate(bool Value);
	void SetDie(bool Value);
	void SetAttack(bool Value);
	void SetHit(bool Value);
	void SetStamping(bool Value);
	void SetJumpStamping(bool Value);
	void SetShield(bool Value);
	void SetShieldDash(bool Value);
	void SetShieldDashLoop(bool Value);

public:
	virtual void Start();
	virtual bool Init();
	virtual void PostUpdate(float DeltaTime);
};

