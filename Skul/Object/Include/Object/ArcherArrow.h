#pragma once
#include "GameObject.h"
#include "Interface.h"

class CArcherArrow :
    public CGameObject, public IObjectView, public IAttackable
{
	friend CScene;

protected:
	CArcherArrow();
	CArcherArrow(const CArcherArrow& obj);
	virtual ~CArcherArrow();

protected:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	CSharedPtr<class CColliderBox2D> m_Body;

private:
	float	m_Speed;

public:
	void SetDirection(Object_View Direction)
	{
		m_View = Direction;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);

	virtual CArcherArrow* Clone();

public:
	void CollisionBeginBlock(const HitResult& Result, class CCollider* Collider);
	void CollisionBeginOverlap(const HitResult& Result, class CCollider* Collider);
};

