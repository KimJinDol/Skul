#pragma once
#include "Resource\Animation2D.h"
#include "../Object/Skul.h"

class CSkulAnimation :
    public CAnimation2D
{
	friend class CSpriteComponent;

protected:
	CSkulAnimation();
	CSkulAnimation(const CSkulAnimation& anim);
	virtual ~CSkulAnimation();

private:
	CSharedPtr<CSkul>	m_pSkul;

	bool m_bIsEvent;

	int	m_AttackComboCount;

	bool	m_bIsJumpAttack;
	bool	m_bIsDash;
	bool	m_bIsSkill1;
	bool	m_bIsSkill2;
	bool	m_bIsSwitch;
	bool	m_bIsIlseom;

public:
	void SetEventMode(bool Value);

public:
	void SetIlseom(bool Value);
	void SetSwitch(bool Value);
	void SetSkill2(bool Value);
	void SetSkill1(bool Value);
	void SetAttackComboCount(int Count);
	void SetJumpAttackOn(bool Value);
	void SetDash(bool Value);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

