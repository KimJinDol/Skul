#pragma once
#include "Resource\Animation2D.h"
#include "../Object/OgreAI.h"

class COgreAnimation :
    public CAnimation2D
{
	friend class CSpriteComponent;

protected:
	COgreAnimation();
	COgreAnimation(const COgreAnimation& anim);
	virtual ~COgreAnimation();

private:
	CSharedPtr<COgreAI>	m_pOgre;

	bool	m_bIsAttack;
	bool	m_bIsDie;

public:
	void SetDie(bool Value);
	void SetAttack(bool Value);

public:
	virtual void Start();
	virtual bool Init();
	virtual void PostUpdate(float DeltaTime);
};

