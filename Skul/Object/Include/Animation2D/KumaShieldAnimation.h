#pragma once
#include "Resource\Animation2D.h"
class CKumaShieldAnimation :
    public CAnimation2D
{
	friend class CSpriteComponent;

protected:
	CKumaShieldAnimation();
	CKumaShieldAnimation(const CKumaShieldAnimation& anim);
	virtual ~CKumaShieldAnimation();

private:
	bool	m_bIsOn;

public:
	void SetOn(bool Value);
	void SetOff(bool Value);

public:
	virtual void Start();
	virtual bool Init();
	virtual void PostUpdate(float DeltaTime);
};

