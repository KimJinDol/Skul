#pragma once
#include <combaseapi.h>
#include "Flag.h"

interface IObjectView
{
	IObjectView()
	{
		m_View = Object_View::Right;
	}

protected:
	Object_View m_View;

public:
	Object_View GetView() const
	{
		return m_View;
	}
	void SetView(Object_View View)
	{
		m_View = View;
	}
};

interface IHitable
{

public:
	virtual float TakeDamage(float Damage, CGameObject* DamageObject) = 0;
};

interface IAttackable
{
protected:
	float m_AD;
};