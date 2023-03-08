#pragma once
#include "ObjectComponent.h"

class CHPComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CHPComponent();
	virtual ~CHPComponent();


private:
	float	m_Hp;
	float	m_MaxHp;
	bool	m_bIsDie;

	std::function<void()> m_OnDieDelegate;

public:
	float SetHP(float Hp);
	void SetMaxHp(float Hp);
	void SetToMaxHp(); 

	void AddHp(float Hp);
	void AddMaxHp(float Hp);

	float GetHp() const;
	float GetMaxHp() const;
	float GetHpRatio() const;
	bool IsDie() const;

	float TakeDamage(float Damage);

public:
	template <typename T>
	void SetDieDelegate(T* pObj, void(T::* pFunc)())
	{
		m_OnDieDelegate = std::bind(pFunc, pObj);
	}
};

