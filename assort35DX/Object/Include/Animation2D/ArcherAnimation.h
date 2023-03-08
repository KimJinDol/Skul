#pragma once
#include "Resource\Animation2D.h"
class CArcherAnimation :
    public CAnimation2D
{
	friend class CSpriteComponent;

protected:
	CArcherAnimation();
	CArcherAnimation(const CArcherAnimation& anim);
	virtual ~CArcherAnimation();

private:
	bool	m_bIsAttack;
	bool	m_bIsDie;
	bool	m_bIsHit;

public:
	void SetDie(bool Value);
	void SetAttack(bool Value);
	void SetHit(bool Value);

public:
	virtual void Start();
	virtual bool Init();
	virtual void PostUpdate(float DeltaTime);
};

